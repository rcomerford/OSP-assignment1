/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"
#include "string.h"
#include <cstdlib>
#include <chrono>

#define MIN_NUM_ARGS 3
#define MAX_NUM_ARGS 5

#define DEBUG_FLAG  "-d"
#define TIMING_FLAG "-t"

#define MILLISECOND_MULTIPLIER 1000
#define MILLISECOND_SYMBOL "ms"

using std::string;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

/**
 * Global variables.
*/
bool IS_DEBUG_MODE = false;
bool IS_TIMING_MODE = false;

int main(int argc, char** argv)
{
    // command line argument validation
    if(
        argc < MIN_NUM_ARGS || 
        argc > MAX_NUM_ARGS
    ){
        cout << "ERROR:\t" << "Program must be run in format \"./copier <infile> <outfile> [-t] [-d]\"." << '\n';
    }
    else if(
        argc > MIN_NUM_ARGS &&        // if flags are present
        argc == (MIN_NUM_ARGS + 1) && // if only one flag
        (strcmp(argv[3], DEBUG_FLAG) != 0 && strcmp(argv[3], TIMING_FLAG) != 0) // if invalid
    ){
        cout << "ERROR:\t" << "Invalid optionals flag (one present)." << '\n';
    }
    else if(
        argc > MIN_NUM_ARGS &&        // if flags are present
        argc == (MIN_NUM_ARGS + 2) && // if two flags
        ((strcmp(argv[3], DEBUG_FLAG) != 0 && strcmp(argv[3], TIMING_FLAG) != 0) || // if invalid first
        (strcmp(argv[4], DEBUG_FLAG) != 0 && strcmp(argv[4], TIMING_FLAG) != 0)) // if invalid second
    ){
        cout << "ERROR:\t" << "Invalid optionals flag (two present)." << '\n';
    }
    else
    {
        // create timing variables
        clock_t read_start, read_end, read_total,
                write_start, write_end, write_total, 
                total_start, total_end, total;
        double read_time, write_time, total_time;

        // optional flags
        if(argc == (MIN_NUM_ARGS + 1))
        {
            if(strcmp(argv[3], DEBUG_FLAG) == 0)  IS_DEBUG_MODE = true;
            if(strcmp(argv[3], TIMING_FLAG) == 0) IS_TIMING_MODE = true;
        }
        else if(argc == (MIN_NUM_ARGS + 2))
        {
            if(strcmp(argv[3], DEBUG_FLAG) == 0)  IS_DEBUG_MODE = true;
            if(strcmp(argv[3], TIMING_FLAG) == 0) IS_TIMING_MODE = true;
            if(strcmp(argv[4], DEBUG_FLAG) == 0)  IS_DEBUG_MODE = true;
            if(strcmp(argv[4], TIMING_FLAG) == 0) IS_TIMING_MODE = true;
        }

        // START TOTAL TIMING
        total_start = clock();

        // initialise classes
        cout << "MAIN:\t" << "Beginning object creation!" << '\n';
        
        string input_file_name = argv[1];
        string output_file_name = argv[2];
        
        writer* main_writer = new writer(output_file_name);
        reader* main_reader = new reader(input_file_name, *main_writer);

        // run reader/writer
        cout << "MAIN:\t" << "Waiting ..." << '\n';

        // start reader timing 
        if(IS_TIMING_MODE) read_start = clock(); 

        // RUN READER
        main_reader->run();

        if(IS_TIMING_MODE)
        {
            // end reader timing 
            read_end = clock();  
            read_total = read_end - read_start;
            read_time = ((double)read_total / (double)CLOCKS_PER_SEC) * MILLISECOND_MULTIPLIER;
        }

        // start writing timing 
        if(IS_TIMING_MODE) write_start = clock(); 

        // RUN WRITER
        main_writer->run();

        if(IS_TIMING_MODE)
        {
            // end writing timing 
            write_end = clock();  
            write_total = write_end - write_start;
            write_time = ((double)write_total / (double)CLOCKS_PER_SEC) * MILLISECOND_MULTIPLIER;
        }

        // END TOTAL TIMING 
        total_end = clock();  
        total = total_end - total_start;
        total_time = ((double)total / (double)CLOCKS_PER_SEC) * MILLISECOND_MULTIPLIER;

        if(IS_TIMING_MODE)
        {

            // OUTPUT TIMING
            cout << "--------------------------------------------------" << "\n";
            cout << "TIMING:\t" << "TOT Reading:         " << read_time << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "TOT Writing:         " << write_time << MILLISECOND_SYMBOL << '\n';
            cout << "TIMING:\t" << "Total:               " << total_time << MILLISECOND_SYMBOL << '\n';
            cout << "--------------------------------------------------" << "\n";

        }
        else
        {
            // always display total time to quantify clock() overhead
            cout << "MAIN:\t" << "Total time: " << total_time << MILLISECOND_SYMBOL << '\n';
        }

        cout << "MAIN:\t" << "Done!" << '\n';

        // memory cleanup
        delete main_writer;
        delete main_reader;
    }

    return EXIT_SUCCESS;
}