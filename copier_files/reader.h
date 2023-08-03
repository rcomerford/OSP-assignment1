/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <functional>

#ifndef READER
#define READER

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::getline;

class reader 
{
    public:
        /* create a reader that reads each line of the file and appends it to the
        * writer's queue
        */
        reader(const string& file_name, writer& my_writer);
        
        /* perform the reading from the file */
        void run();

        /**
         * 
        */
        void handleLine(const string& line);

    private:
        ifstream in;
        writer& the_writer;
};

#endif // READER