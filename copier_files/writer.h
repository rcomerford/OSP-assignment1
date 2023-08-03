/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include <fstream>
#include <iostream>
#include <string>
#include <deque>

#ifndef WRITER
#define WRITER

using std::string;
using std::cout;
using std::endl;
using std::ofstream;

class writer 
{
    public:
        /**
         * Creates a "writer" instance.
         * file_name - The file name argument given in the cli.
        */
        writer(const string& file_name);

        /**
         * Does the actual writing from the queue of lines.
         **/
        void run();

        /**
         * Appends a line to the queue to be written.
         * line - The line to be added to the queue.
        */
        void append(const string& line);

        /**
         * 
        */
       void writeNextLine();

    private:
        /**
         * The output stream doing the writing.
        */
        std::ofstream out;

        /**
         * The queue of lines to be written.
        */
        std::deque<string> queue;
};

#endif //WRITER