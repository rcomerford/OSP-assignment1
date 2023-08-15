/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include "reader.h"

// "constant" static variables
bool writer::IS_DEBUG_MODE;
unsigned writer::MAX_QUEUE_SIZE;

// initialise static variables
ofstream writer::out;
deque<string> writer::queue;
unsigned writer::queue_size = 0;

bool writer::reading_finished = false;

bool writer::init(const std::string& FILE_NAME, const unsigned& MAX_QUEUE_ARG, const bool& DEBUG_MODE_ARG)
{
    // static variables
    MAX_QUEUE_SIZE = MAX_QUEUE_ARG;
    IS_DEBUG_MODE = DEBUG_MODE_ARG;
    if(IS_DEBUG_MODE) cout << "WRITER INIT:\tInitialised with file name: " << FILE_NAME << '\n';

    // attempt to open file and return status
    out.open(FILE_NAME.c_str());
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
    while(true)
    {
        pthread_mutex_lock(&reader::queue_lock);
        {
            // if queue is empty, await produced addition
            while(!reading_finished && writer::is_queue_empty())
                pthread_cond_wait(&reader::item_added_signal, &reader::queue_lock);

            string line = "";

            if(reading_finished && writer::is_queue_empty())
            {
                pthread_mutex_unlock(&reader::queue_lock);
                break;
            }
            else
            {
                // write file and remove from queue
                writer::remove(line);
                out <<  line;
            }
        }
        pthread_mutex_unlock(&reader::queue_lock);

        // send: something has been removed from the list
        pthread_cond_signal(&reader::item_removed_signal);
    }

    if(IS_DEBUG_MODE) cout << "WRITER THREAD:\tClosing thread with ID: " << pthread_self() << '\n';
    return nullptr;
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