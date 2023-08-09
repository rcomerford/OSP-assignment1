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

/**
 * TODO remove??
 * Please note that methods and data need to be static as there's a
 * variety of information that needs to be coordinated between writers
 * such as locks and other shared data. Also, as the pthread library
 * is a c library, it does not know about classes so runner in particular
 * needs to be static. You can pass in instances into the function as
 * pointers though.
 **/

class writer 
{
    public:

        /**
		 * Initialise the shared data for the class.
		 */
        static void init(const string& file_name);

		/**
		 * Thread function which does the actual writing to the output file.
		 **/
        static void* runner(void*);

        /**
         * Does the setup for and launches the thread.
        */
        pthread_t run();

        /**
         * Appends a line of text to the shared writing queue.
        */
        static void append(const string& line);

        /**
         * TODO
        */
       static string pop();

       /**
        * TODO
       */
      static void write(string& line);

    private:

        /**
         * The output stream doing the writing.
        */
        static ofstream out;

        /**
         * The queue of lines to be written.
        */
        static deque<string> queue;

        /**
         * TODO add
        */
        static pthread_mutex_t queue_lock;

        /**
         * TODO add
        */
        static pthread_mutex_t write_lock;

};

#endif // WRITER