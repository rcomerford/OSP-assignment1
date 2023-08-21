/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include "reader.h"

#define KILOBYTE 1024
#define MEGABYTE 1048576

#define BUFFER_LENGTH KILOBYTE

// "constant" static variables
bool writer::IS_DEBUG_MODE;
unsigned writer::MAX_QUEUE_SIZE;

// initialise static variables
ofstream writer::out;
deque<const char*> writer::queue;
unsigned writer::queue_size = 0;

bool writer::reading_finished = false;

bool writer::init(const char* FILE_NAME, const unsigned& MAX_QUEUE_ARG, const bool& DEBUG_MODE_ARG)
{
    // static variables
    MAX_QUEUE_SIZE = MAX_QUEUE_ARG;
    IS_DEBUG_MODE = DEBUG_MODE_ARG;
    if(IS_DEBUG_MODE) cout << "WRITER INIT:\tInitialised with file name: " << FILE_NAME << '\n';

    // attempt to open file and return status
    out.open(FILE_NAME);
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
    char* buffer = new char[BUFFER_LENGTH];

    while(true)
    {
        pthread_mutex_lock(&reader::queue_lock);
        {
            // if queue is empty, await produced addition
            while(!reading_finished && writer::is_queue_empty())
                pthread_cond_wait(&reader::item_added_signal, &reader::queue_lock);

            if(reading_finished && writer::is_queue_empty())
            {
                pthread_mutex_unlock(&reader::queue_lock);
                break;
            }
            else
            {
                // write file and remove from queue
                writer::remove(buffer);
                out <<  buffer;
            }
        }
        // send: something has been removed from the list
        pthread_cond_signal(&reader::item_removed_signal);

        pthread_mutex_unlock(&reader::queue_lock);
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

void writer::append(const char* line)
{
    queue.push_back(line);

    if(IS_DEBUG_MODE) cout << "WRITER THREAD:\tAppended line to queue." << '\n';
    queue_size++;
}

void writer::remove(char* line)
{
    line = (char*)queue.front();
    queue.pop_front();
    queue_size--;
}

void writer::mark_reading_finished()
{
    reading_finished = true;
}