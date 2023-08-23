/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include <pthread.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>

#ifndef WRITER
#define WRITER

using std::ofstream;
using std::deque;
using std::cout;
using std::endl;
using std::string;
using std::ios;

class writer 
{
    public:

        /**
		 * Initialise the shared data for the class.
		 */
        static bool init(
            const string& FILE_NAME, 
            const unsigned& MAX_QUEUE_SIZE);

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
        static void append(const string& line);
        static void remove(string& line);

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

		/**
		 * Timing variables.
		*/
		static clock_t running_timing_lock; 
		static unsigned running_total_lock;
		static clock_t running_timing_signal; 
		static unsigned running_total_signal;
		static clock_t running_timing_write; 

    private:

        /**
         * The output stream doing the writing.
        */
        static ofstream out;

        /**
         * Related to the queue of lines to be written.
        */
        static deque<string> queue;
        static unsigned queue_size;
        static unsigned MAX_QUEUE_SIZE;

};

#endif // WRITER