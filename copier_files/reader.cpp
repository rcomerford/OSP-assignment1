/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"

#define KILOBYTE 1024
#define MEGABYTE 1048576

#define BUFFER_LENGTH KILOBYTE

reader::reader(const char* FILE_NAME, writer& MAIN_WRITER) : main_writer(MAIN_WRITER)
{
    cout << "READER:\tInitialised with file name: " << FILE_NAME << '\n';
    in.open(FILE_NAME);
}

void reader::run()
{
    if(in.is_open()) 
    {
        char* buffer = new char[BUFFER_LENGTH];

        // repeatedly read next line of characters from input stream.
        while(in.read(buffer, BUFFER_LENGTH)) 

            // and add to writer queue.
            main_writer.append(buffer);

        in.close();
    } 
    else
    {
        // if file failed to open
        cout << "ERROR:\t" << "Input file failed to open." << '\n';
    }
}