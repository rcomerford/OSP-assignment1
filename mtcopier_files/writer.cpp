/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include "reader.h"

// global variables
extern bool IS_DEBUG_MODE;
extern bool IS_TIMING_MODE;

// "constant" static variables
unsigned writer::MAX_QUEUE_SIZE;

// initialise static variables
ofstream writer::out;
deque<string> writer::queue;
unsigned writer::queue_size = 0;
bool writer::reading_finished = false;

// static timing variables
clock_t writer::running_timing_lock; 
unsigned writer::running_total_lock = 0;
clock_t writer::running_timing_signal; 
unsigned writer::running_total_signal = 0;
clock_t writer::running_timing_write;

bool writer::init(const string& FILE_NAME, const unsigned& MAX_QUEUE_ARG)
{
    // static variables
    MAX_QUEUE_SIZE = MAX_QUEUE_ARG;
    if(IS_DEBUG_MODE) cout << "WRITER INIT:\tInitialised with file name: " << FILE_NAME << '\n';

    // attempt to open file and return status
    out.open(FILE_NAME, std::ios::binary);
    if(!out) return false;
    return true;
}

pthread_t writer::run()
{
    // create new thread
    pthread_t pt_id;
    if(IS_DEBUG_MODE) cout << "WRITER THREAD:\tCreated thread with ID: " << pt_id << '\n';
    pthread_create(&pt_id, nullptr, &runner, nullptr);
    return pt_id;
}

void* writer::runner(void* arg)
{
    // timing variables
    clock_t  lock_timing = 0;
    unsigned lock_count = 0;
    clock_t  signal_timing = 0;
    unsigned signal_count = 0;
    clock_t  write_timing = 0;

    string line = "";

    while(true)
    {
        clock_t lock_start, lock_end, 
                write_start, write_end,
                signal_start, signal_end;

        if(IS_TIMING_MODE)
        {
            // START SIGNAL WAIT TIMING  
            lock_start = clock();
        }

        pthread_mutex_lock(&reader::queue_lock);
        {
            if(IS_TIMING_MODE)
            {
                // END SIGNAL WAIT TIMING 
                lock_end = clock();  
                lock_timing += lock_end - lock_start;
                lock_count++;
            }

            if(IS_TIMING_MODE)
            {
                // START SIGNAL WAIT TIMING  
                signal_start = clock();
            }

            // if queue is empty, await produced addition
            while(!reading_finished && writer::is_queue_empty())
                pthread_cond_wait(&reader::item_added_signal, &reader::queue_lock);

            if(IS_TIMING_MODE)
            {
                // END SIGNAL WAIT TIMING 
                signal_end = clock();  
                signal_timing += signal_end - signal_start;
                signal_count++;
            }

            if(reading_finished && writer::is_queue_empty())
            {
                pthread_mutex_unlock(&reader::queue_lock);
                break;
            }
            else
            {
                if(IS_TIMING_MODE)
                {
                    // START READ WAIT TIMING  
                    write_start = clock();
                }

                // write file and remove from queue
                writer::remove(line);
                // write to output
                out << line;

                if(IS_DEBUG_MODE) cout << "WRITER THREAD:\tWritten new line." << '\n';

                if(IS_TIMING_MODE)
                {
                    // END READ TIMING 
                    write_end = clock();  
                    clock_t duration = write_end - write_start;
                    write_timing += duration;
                }
            }
        }
        // send: something has been removed from the list
        pthread_cond_signal(&reader::item_removed_signal);

        pthread_mutex_unlock(&reader::queue_lock);
    }

    if(IS_DEBUG_MODE) cout << "WRITER THREAD:\tClosing thread with ID: " << pthread_self() << '\n';

    double* results = new double[3];

    // lock average
    results[0] = (double)lock_timing / (double)lock_count;
    // signal average
    results[1] = (double)signal_timing / (double)signal_count;
    // write total
    results[2] = (double)write_timing; 

    return (void*)results;
}

bool writer::is_queue_full()
{
    return queue_size == MAX_QUEUE_SIZE;
}

bool writer::is_queue_empty()
{
    return queue_size == 0;
}

void writer::append(const string& line)
{
    queue.push_back(line);

    if(IS_DEBUG_MODE) cout << "WRITER THREAD:\tAppended line to queue." << '\n';
    queue_size++;
}

void writer::remove(string& line)
{
    line = queue.front();
    queue.pop_front();
    queue_size--;
}

void writer::mark_reading_finished()
{
    reading_finished = true;
}