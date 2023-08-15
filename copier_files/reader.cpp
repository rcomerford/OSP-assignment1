/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"



reader::reader(const string& FILE_NAME, writer& MAIN_WRITER) : the_writer(MAIN_WRITER)
{
    cout << "READER INIT:\tInitialised with file name: " << FILE_NAME << '\n';
    in.open(FILE_NAME.c_str(), std::ios::in);
}

void reader::run()
{
    if(in.is_open()) 
    {
        string current_line;

        // repeatedly read next line of characters from input stream.
        while (getline(in, current_line)) 
            handleLine(current_line);

        in.close();
    }
}

void reader::handleLine(const string& line)
{
    // add line to writer queue.
    the_writer.append(line);
}