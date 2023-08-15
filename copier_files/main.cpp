/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"
#include <cstdlib>
#include <chrono>

#define NUM_ARGS 3
#define INFILE_ARG 1
#define OUTFILE_ARG 2

using std::string;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

int main(int argc, char** argv)
{
    // command line argument validation
    if(argc != NUM_ARGS)
    {
        cout << "ERROR:\t" << "Program must be run in format \"./copier <infile> <outfile>\"." << '\n';
    }
    else
    {
        // TIMING START
        auto start = high_resolution_clock::now();

        // initialise classes
        cout << "MAIN:\t" << "Initialising reader and writer!" << '\n';
        writer* main_writer = new writer(argv[OUTFILE_ARG]);
        reader* main_reader = new reader(argv[INFILE_ARG], *main_writer);

        // run reader/writer
        cout << "MAIN:\t" << "Waiting ..." << '\n';
        main_reader->run();
        main_writer->run();

        // TIMING END
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        // TIMING CONVERT TO SECONDS
        cout << "MAIN:\t" << "Time: " << duration.count() << "ms" << '\n';
        cout << "MAIN:\t" << "Done!" << '\n';

        // memory cleanup
        delete main_writer;
        delete main_reader;
    }

    return EXIT_SUCCESS;
}