/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include <fstream>
#include <iostream>
#include <string>
#include <deque>

#ifndef WRITER
#define WRITER

using std::cout;
using std::endl;
using std::ofstream;
using std::deque;

class writer 
{
    public:
    
        /**
		 * Initialise the class & open file.
        */
        writer(const char* file_name);

        /**
         * Does the actual writing from the queue of lines.
         **/
        void run();

        /**
         * Appends a line to the queue to be written.
        */
        void append(const char* line);

        /**
         * Handles writing a single line to the output string.
        */
        void writeNextLine();

    private:

        /**
         * The output stream doing the writing.
        */
        ofstream out;

        /**
         * The queue of lines to be written.
        */
        deque<const char*> queue;
};

#endif //WRITER