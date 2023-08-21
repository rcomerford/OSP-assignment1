/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include <pthread.h>
#include <deque>
#include <fstream>
#include <iostream>

#ifndef WRITER
#define WRITER

using std::ofstream;
using std::deque;
using std::cout;
using std::endl;
using std::ios;

class writer 
{
    public:

        /**
		 * Initialise the shared data for the class.
		 */
        static bool init(
            const char* FILE_NAME, 
            const unsigned& MAX_QUEUE_SIZE, 
            const bool& IS_DEBUG_MODE);

        /**
         * Does the setup for and launches the thread.
        */
        pthread_t run();

		/**
		 * Thread which does the actual writing (consuming).
		 **/
        static void* runner(void*);

        /**
         * Queue manipulation.
        */
        static void append(const char* line);
        static void remove(char* line);

        /**
         * Queue state.
        */
        static bool is_queue_full();
        static bool is_queue_empty();

        /*
         * Reader state.
        */
        static bool reading_finished;
        static void mark_reading_finished();

    private:

		/**
		 * Dictates whether the program threads output status updates. 
		*/
		static bool IS_DEBUG_MODE;

        /**
         * The output stream doing the writing.
        */
        static ofstream out;

        /**
         * Related to the queue of lines to be written.
        */
        static deque<const char*> queue;
        static unsigned queue_size;
        static unsigned MAX_QUEUE_SIZE;

};

#endif // WRITER