/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"
#include "string.h"
#include <cstdlib>
#include <chrono>

#define NUMTHREADS_ARG 1
#define INFILE_ARG 2
#define OUTFILE_ARG 3
#define DEBUG_ARG 4
#define DEBUG_FLAG "-d"

#define MIN_NUM_ARGS 4
#define MAX_NUM_ARGS 5

#define MIN_NUM_THREADS 25
#define MAX_NUM_THREADS 100

#define MAX_QUEUE_SIZE 100

using std::cout;
using std::atoi;
using std::isdigit;
using std::string;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

/**
 * Validates INTEGER command-line input.
*/
bool isNumber(const string& s)
{
    bool result = true;
    int size = s.length();

    for(int i = 0; i < size; i++) 
    {
        char ch = s[i];
        if(!isdigit(ch)) 
            result = false;
    }

    return result;
}

/**
 * Main function.
*/
int main(int argc, char** argv) 
{
    // command line variables, initialised with default values
    unsigned  NUM_THREADS = 50;
    bool      IS_DEBUG_MODE = false;

    // command line argument validation
    if(
        argc < MIN_NUM_ARGS || 
        argc > MAX_NUM_ARGS
    ){
        cout << "ERROR:\t" << "Program must be run in format \"./copier <numthreads> <infile> <outfile> [-d]\"." << '\n';
    }
    else if(
        !isNumber(argv[NUMTHREADS_ARG]) || 
        atoi(argv[NUMTHREADS_ARG]) < MIN_NUM_THREADS || 
        atoi(argv[NUMTHREADS_ARG]) > MAX_NUM_THREADS
    ){
        cout << "ERROR:\t" << "<numthreads> must be an INTEGER between (inclusive) " << NUMTHREADS_ARG << " and " << MAX_NUM_THREADS << "." << '\n';
    }
    else if(
        !reader::init(argv[INFILE_ARG], IS_DEBUG_MODE) || 
        !writer::init(argv[OUTFILE_ARG], MAX_QUEUE_SIZE, IS_DEBUG_MODE)
    ){
        cout << "ERROR:\t" << "Files failed to open." << '\n';
    }
    else if(
        argc > MIN_NUM_ARGS && 
        strcmp(argv[DEBUG_ARG], DEBUG_FLAG) != 0
    ){
        cout << "ERROR:\t" << "Invalid optional arguments." << '\n';
    }
    else
    {
        // TIMING START
        auto start = high_resolution_clock::now();

        // command line variables
        NUM_THREADS = atoi(argv[1]);
        IS_DEBUG_MODE = argc > MIN_NUM_ARGS && strcmp(argv[DEBUG_ARG], DEBUG_FLAG) == 0;

        // initialise arrays
        pthread_t* reader_threads = new pthread_t[NUM_THREADS];
        pthread_t* writer_threads = new pthread_t[NUM_THREADS];

        // create threads for each reader & writer
        cout << "MAIN:\t" << "Beginning thread creation!" << '\n';
        for(int i = 0; i < (NUM_THREADS - 1); i++)
        {
            reader new_reader = reader();
            writer new_writer = writer();
            reader_threads[i] = new_reader.run();
            writer_threads[i] = new_writer.run();
        }

        // wait on threads to complete
        cout << "MAIN:\t" << "Waiting for threads ..." << '\n';
        for(int i = 0; i < (NUM_THREADS - 1); i++)
        {
            pthread_join(reader_threads[i], nullptr);
            pthread_join(writer_threads[i], nullptr);
        }

        // TIMING END
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        // TIMING CONVERT TO SECONDS
        cout << "MAIN:\t" << "Time: " << duration.count() << "ms" << '\n';
        cout << "MAIN:\t" << "Done!" << '\n';

        // memory cleanup
        delete[] reader_threads;
        delete[] writer_threads;
    }

    return EXIT_SUCCESS;
}