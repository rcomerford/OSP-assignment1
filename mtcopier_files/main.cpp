/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"
#include <cstdlib>

using std::cout;
using std::endl;
using std::string;
using std::atoi;
using std::isdigit;

// global variables due to using cleanup() function for management
reader* readers;
writer* writers;

void cleanup() 
{
    delete[] readers;
    delete[] writers;
}

// TODO rewrite??
// Charles Salvia - Stack Overflow (https://stackoverflow.com/users/168288/charles-salvia)
bool isNumber(const string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

// TODO implement
bool isValidPath()
{

}

int main(int argc, char** argv) 
{
    atexit(cleanup);

    // command line argument validation
    if(argc != 4)
        cout << "Program must be run in format \"./copier <numthreads> <infile> <outfile>\"." << endl;
    else if(!isNumber(argv[1]) || atoi(argv[1]) < 25 || atoi(argv[1]) > 100)
        cout << "<numthreads> must be an INTEGER between 25 and 100." << endl;
    // else if
    //     // CHECK IF argv[2] AND argv[3] ARE VALID SYSTEM PATHS
    else
    {
        // valid command lune arguments
        const int NUM_THREADS = atoi(argv[1]);
        string input_file_name = argv[2];
        string output_file_name = argv[3];

        readers = new reader[NUM_THREADS];
        writers = new writer[NUM_THREADS];

        reader::init(input_file_name);
        writer::init(output_file_name);

        // create threads for reader & writer
        for(int i = 0; i < (NUM_THREADS - 1); i++)
        {
            readers[i] = reader();
            readers[i].run();

            writers[i] = writer();
            writers[i].run();
        }

        /**
         * have loop here waiting for the threads to complete. Please see
         * section on avoiding busy waiting on the assignment specification to see
         * what need to be done here
        */
    }

    return EXIT_SUCCESS;
}