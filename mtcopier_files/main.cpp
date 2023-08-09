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

// global variables
pthread_t* reader_threads;
pthread_t* writer_threads;

void cleanup() 
{
    // if(reader_threads != nullptr)
    //     delete[] reader_threads;
    // if(writer_threads != nullptr)
    //     delete[] writer_threads;
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
    return false;
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
        // TODO CHECK IF FILE OPENS LOL
    else
    {
        // valid command line arguments
        const int NUM_THREADS = atoi(argv[1]);
        string input_file_name = argv[2];
        string output_file_name = argv[3];

        // initialise thread classes
        reader::init(input_file_name);
        writer::init(output_file_name);

        // initialise arrays
        reader_threads = new pthread_t[NUM_THREADS];
        writer_threads = new pthread_t[NUM_THREADS];

        // create threads for each reader & writer
        // adding each thread to the global variable
        for(int i = 0; i < (NUM_THREADS - 1); i++)
        {
            reader new_reader = reader();
            reader_threads[i] = new_reader.run();

            writer new_writer = writer();
            writer_threads[i] = new_writer.run();
        }

        /**
         * have loop here waiting for the threads to complete. Please see
         * section on avoiding busy waiting on the assignment specification to see
         * what need to be done here
        */

        for(int i = 0; i < (NUM_THREADS - 1); i++)
        {
            pthread_join(reader_threads[i], NULL);
            pthread_join(writer_threads[i], NULL);
        }

        cout << "DONE!" << endl;
    }

    return EXIT_SUCCESS;
}