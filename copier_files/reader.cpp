/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"

// global variables
extern bool IS_DEBUG_MODE;

reader::reader(const string& FILE_NAME, writer& MAIN_WRITER) : main_writer(MAIN_WRITER)
{
    if(IS_DEBUG_MODE) cout << "READER:\tTrying to initialise with file name: " << FILE_NAME << '\n';
    in.open(FILE_NAME.c_str(), std::ios::in);
}

void reader::run()
{
    if(in.is_open()) 
    {
        string current_line;

        // repeatedly read next line of characters from input stream.
        while (getline(in, current_line)) 
        {
            if(IS_DEBUG_MODE) cout << "READER:\tRead new line." << '\n';

            // and add to writer queue.
            main_writer.append(current_line);
        }

        in.close();
    } 
    else
    {
        // if file failed to open
        cout << "ERROR:\t" << "Input file failed to open." << '\n';
    }
}