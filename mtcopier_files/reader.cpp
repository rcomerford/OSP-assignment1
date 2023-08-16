/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"

// "constant" static variables
bool reader::IS_DEBUG_MODE;

// initialise static variables
ifstream reader::in;
pthread_mutex_t reader::queue_lock;
pthread_cond_t reader::item_removed_signal;
pthread_cond_t reader::item_added_signal;

bool reader::init(const string& FILE_NAME, const bool& DEBUG_MODE_ARG)
{
    // static variables
    IS_DEBUG_MODE = DEBUG_MODE_ARG;
    if(IS_DEBUG_MODE) cout << "READER INIT:\tInitialised with file name: " << FILE_NAME << '\n';

    // initialise mutex & condition variables
    pthread_mutex_init(&queue_lock, nullptr);
    pthread_cond_init(&item_removed_signal, nullptr);
    pthread_cond_init(&item_added_signal, nullptr);

    // attempt to open file and return status
    in.open(FILE_NAME.c_str());
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
    string line = "";

    while(!writer::reading_finished)
    {
        pthread_mutex_lock(&queue_lock);
        {
            // if queue is full, await consumer removal
            while(writer::is_queue_full())
                pthread_cond_wait(&item_removed_signal, &queue_lock);

            // read file and add to queue
            if(getline(in, line))
            {
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
    return nullptr;
}