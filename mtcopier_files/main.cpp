/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"
#include "string.h"
#include <cstdlib>
#include <chrono>

#define MIN_NUM_ARGS 4
#define MAX_NUM_ARGS 6

#define DEBUG_FLAG  "-d"
#define TIMING_FLAG "-t"

#define MIN_NUM_THREADS 25
#define MAX_NUM_THREADS 100

#define MAX_QUEUE_SIZE 100

#define MILLISECOND_MULTIPLIER 1000
#define MILLISECOND_SYMBOL "ms"

using std::cout;
using std::string;
using std::atoi;
using std::isdigit;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

/**
 * Global variables.
*/
bool IS_DEBUG_MODE = false;
bool IS_TIMING_MODE = false;

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
    // command line argument validation
    if(
        argc < MIN_NUM_ARGS || 
        argc > MAX_NUM_ARGS
    ){
        cout << "ERROR:\t" << "Program must be run in format \"./mtcopier <numthreads> <infile> <outfile> [-t] [-d]\"." << '\n';
    }
    else if(
        !isNumber(argv[1]) || 
        atoi(argv[1]) < MIN_NUM_THREADS || 
        atoi(argv[1]) > MAX_NUM_THREADS
    ){
        cout << "ERROR:\t" << "<numthreads> must be an INTEGER between (inclusive) " << MIN_NUM_THREADS << " and " << MAX_NUM_THREADS << "." << '\n';
    }
    else if(
        !reader::init(argv[2]) || 
        !writer::init(argv[3], MAX_QUEUE_SIZE)
    ){
        cout << "ERROR:\t" << "Files failed to open." << '\n';
    }
    else if(
        argc > MIN_NUM_ARGS &&        // if flags are present
        argc == (MIN_NUM_ARGS + 1) && // if only one flag
        (strcmp(argv[4], DEBUG_FLAG) != 0 && strcmp(argv[4], TIMING_FLAG) != 0) // if invalid
    ){
        cout << "ERROR:\t" << "Invalid optionals flag (one present)." << '\n';
    }
    else if(
        argc > MIN_NUM_ARGS &&        // if flags are present
        argc == (MIN_NUM_ARGS + 2) && // if two flags
        ((strcmp(argv[4], DEBUG_FLAG) != 0 && strcmp(argv[4], TIMING_FLAG) != 0) || // if invalid first
        (strcmp(argv[5], DEBUG_FLAG) != 0 && strcmp(argv[5], TIMING_FLAG) != 0)) // if invalid second
    ){
        cout << "ERROR:\t" << "Invalid optionals flag (two present)." << '\n';
    }
    else
    {

        // create timing variables
        double lock_time = 0;
        double signal_time = 0;
        double read_time = 0;
        double write_time = 0;
        // std::chrono used here so as not to duplicate thread clocks
        auto total_start = high_resolution_clock::now();

        // command line variables
        const unsigned NUM_THREADS = atoi(argv[1]);

        // optional flags
        if(argc == (MIN_NUM_ARGS + 1))
        {
            if(strcmp(argv[4], DEBUG_FLAG) == 0)  IS_DEBUG_MODE = true;
            if(strcmp(argv[4], TIMING_FLAG) == 0) IS_TIMING_MODE = true;
        }
        else if(argc == (MIN_NUM_ARGS + 2))
        {
            if(strcmp(argv[4], DEBUG_FLAG) == 0)  IS_DEBUG_MODE = true;
            if(strcmp(argv[4], TIMING_FLAG) == 0) IS_TIMING_MODE = true;
            if(strcmp(argv[5], DEBUG_FLAG) == 0)  IS_DEBUG_MODE = true;
            if(strcmp(argv[5], TIMING_FLAG) == 0) IS_TIMING_MODE = true;
        }

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

            void* reader_result;
            void* writer_result;

            pthread_join(reader_threads[i], &reader_result);
            pthread_join(writer_threads[i], &writer_result);

            // ADD RETURNED TIMING TO CUMULATIVE VARIABLES
            lock_time += ((double*)reader_result)[0];
            lock_time += ((double*)writer_result)[0];

            signal_time += ((double*)reader_result)[1];
            signal_time += ((double*)writer_result)[1];
            
            read_time += ((double*)reader_result)[2];
            write_time += ((double*)writer_result)[2];

            // CLEAN UP MEMORY
            delete[] (double*)reader_result;
            delete[] (double*)writer_result;
        }

        // TIMING END
        auto total_end = high_resolution_clock::now();
        // microseconds used for higher fidelity once converted to milli
        auto micro_total = duration_cast<microseconds>(total_end - total_start);
        double total = (double)micro_total.count() / MILLISECOND_MULTIPLIER;

        if(IS_TIMING_MODE)
        {
            // acquiring lock
            double converted_lock_time = lock_time / (double)CLOCKS_PER_SEC;
            double average_lock = (converted_lock_time / (2*NUM_THREADS))  * MILLISECOND_MULTIPLIER;
            double total_lock = converted_lock_time * MILLISECOND_MULTIPLIER;

            // awaiting signal
            double converted_signal_time = signal_time / (double)CLOCKS_PER_SEC;
            double average_signal = (converted_signal_time / (2*NUM_THREADS))  * MILLISECOND_MULTIPLIER;
            double total_signal = converted_signal_time * MILLISECOND_MULTIPLIER;

            // total reading
            double tot_read_time = ((double)read_time / (double)CLOCKS_PER_SEC) * MILLISECOND_MULTIPLIER;

            // total writing
            double tot_write_time = ((double)write_time / (double)CLOCKS_PER_SEC) * MILLISECOND_MULTIPLIER;

            // OUTPUT TIMING
            cout << "--------------------------------------------------" << "\n";
            cout << "TIMING:\t" << "AVG Acquiring Lock:  " << average_lock << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "TOT Acquiring Lock:  " << total_lock << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "AVG Awaiting Signal: " << average_signal << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "TOT Awaiting Signal: " << total_signal << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "TOT Reading:         " << tot_read_time << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "TOT Writing:         " << tot_write_time << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "Total:               " << total << MILLISECOND_SYMBOL << '\n';
            cout << "--------------------------------------------------" << "\n";

        }
        else
        {
            // always display total time to quantify clock() overhead
            cout << "MAIN:\t" << "Total time: " << total << MILLISECOND_SYMBOL << '\n';
        }

        cout << "MAIN:\t" << "Done!" << '\n';

        // memory cleanup
        delete[] reader_threads;
        delete[] writer_threads;
    }

    return EXIT_SUCCESS;
}