/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include "reader.h"

extern bool IS_DEBUG_MODE;
extern bool IS_TIMING_MODE;

// initialise static variables
ifstream reader::in;
pthread_mutex_t reader::queue_lock;
pthread_cond_t reader::item_removed_signal;
pthread_cond_t reader::item_added_signal;

bool reader::init(const string& FILE_NAME)
{
    if(IS_DEBUG_MODE) cout << "READER INIT:\tInitialised with file name: " << FILE_NAME << '\n';

    // initialise mutex & condition variables
    pthread_mutex_init(&queue_lock, nullptr);
    pthread_cond_init(&item_removed_signal, nullptr);
    pthread_cond_init(&item_added_signal, nullptr);

    // attempt to open file and return status
    in.open(FILE_NAME, std::ios::binary);
    if(!in) return false;
    return true;
}

pthread_t reader::run()
{
    // create new thread
    pthread_t pt_id;
    if(IS_DEBUG_MODE) cout << "READER THREAD:\tCreated thread with ID: " << pt_id << '\n';
    pthread_create(&pt_id, nullptr, runner, nullptr);
    return pt_id;
}

void* reader::runner(void* arg)
{
    // timing variables
    clock_t  lock_timing = 0;
    unsigned lock_count = 0;
    clock_t  signal_timing = 0;
    unsigned signal_count = 0;
    clock_t  read_timing = 0;

    string line = "";

    while(!writer::reading_finished)
    {
        clock_t lock_start, lock_end, 
                read_start, read_end,
                signal_start, signal_end;
        
        if(IS_TIMING_MODE)
        {
            // START SIGNAL WAIT TIMING  
            lock_start = clock();
        }
        
        pthread_mutex_lock(&queue_lock);
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

            // if queue is full, await consumer removal
            while(writer::is_queue_full())
                pthread_cond_wait(&item_removed_signal, &queue_lock);
                
            if(IS_TIMING_MODE)
            {
                // END SIGNAL WAIT TIMING 
                signal_end = clock();  
                signal_timing += signal_end - signal_start;
                signal_count++;
            }
            
            if(IS_TIMING_MODE)
            {
                // START READ WAIT TIMING  
                read_start = clock();
            }

            // read file and add to queue
            if(getline(in, line))
            {
                if(IS_TIMING_MODE)
                {
                    // END READ TIMING 
                    read_end = clock();  
                    clock_t duration = read_end - read_start;
                    read_timing += duration;
                }

                if(IS_DEBUG_MODE) cout << "READER THREAD:\tRead new line" << '\n';
                
                // add newly read line to output queue
                writer::append(line);

                // append newline character to output queue unless at final line
                if(in.good())
                    writer::append("\n");
            }
            else 
            {
                writer::mark_reading_finished();

                // slightly hacky, because the break skips the unlock & signal below
                pthread_mutex_unlock(&queue_lock);
                pthread_cond_signal(&item_added_signal);
                break;
            }
        }
        // send: something has been added to the list
        pthread_cond_signal(&item_added_signal);
        pthread_mutex_unlock(&queue_lock);
    }
    if(IS_DEBUG_MODE) cout << "READER THREAD:\tClosing thread with ID: " << pthread_self() << '\n';

    double* results = new double[3];

    // lock average
    results[0] = (double)lock_timing / (double)lock_count;
    // signal average
    results[1] = (double)signal_timing / (double)signal_count;
    // read total
    results[2] = (double)read_timing; 

    return (void*)results;
}