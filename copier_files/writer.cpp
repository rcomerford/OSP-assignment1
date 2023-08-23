/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"

// global variables
extern bool IS_DEBUG_MODE;

writer::writer(const string& FILE_NAME)
{
    if(IS_DEBUG_MODE) cout << "WRITER:\tTrying to initialise with file name: " << FILE_NAME << '\n';
    out.open(FILE_NAME);
}

void writer::run()
{
    if(out.is_open()) 
    {
        // write lines from queue until empty
        while(!queue.empty())
        {
            writeNextLine();
        }

        out.close();
    } 
    else
    {
        // if file failed to open
        cout << "ERROR:\t" << "Output file failed to open." << '\n';
    }
}

void writer::append(const string& line)
{
    queue.push_back(line);
}

void writer::writeNextLine()
{
    string next_string = queue.front();
    out << next_string;
    queue.pop_front();  

    // output endline char for every line except the last.
    if(!queue.empty())
        out << endl;

    if(IS_DEBUG_MODE) cout << "WRITER:\tWritten new line." << '\n';
}