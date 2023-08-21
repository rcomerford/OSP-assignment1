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

		/**
		 * Initialise the class & open file.
		 */
        reader(
            const string& FILE_NAME, 
            writer& MAIN_WRITER);
        
        /**
		 * Launches single threaded reading.
		 */
        void run();

    private:

        /**
         * The input stream doing the reading.
        */
        ifstream in;

        /**
         * The writer object doing the writing.
        */
        writer& main_writer;
};

#endif // READER