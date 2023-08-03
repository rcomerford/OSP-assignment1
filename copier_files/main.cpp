/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"
#include <cstdlib>

using std::string;

int main(int argc, char** argv)
{
    // command line argument validation
    if(argc != 3)
    {
        cout << "Program must be run in format \"./copier <infile> <outfile>\"." << endl;
    }
    else
    {
        string input_file_name = argv[1];
        string output_file_name = argv[2];

        writer* main_writer = new writer(output_file_name);
        reader* main_reader = new reader(input_file_name, *main_writer);

        main_reader->run();
        main_writer->run();

        delete main_writer;
        delete main_reader;
    }

    return EXIT_SUCCESS;
}