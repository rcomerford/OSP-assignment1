/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"

#define KILOBYTE 1024
#define MEGABYTE 1048576

#define BUFFER_LENGTH KILOBYTE

writer::writer(const char* FILE_NAME)
{
    cout << "WRITER:\tInitialised with file name: " << FILE_NAME << '\n';
    out.open(FILE_NAME);
}

void writer::run()
{
    while(!queue.empty())
        writeNextLine();

    if(out.is_open()) 
    {
        // write lines from queue until empty
        while(!queue.empty())
            writeNextLine();

        out.close();
    } 
    else
    {
        // if file failed to open
        cout << "ERROR:\t" << "Output file failed to open." << '\n';
    }
}

void writer::append(const char* line)
{
    queue.push_back(line);
}

void writer::writeNextLine()
{
    char* buffer = (char*)queue.front();
    out << buffer;
    queue.pop_front();  

    // output endline char for every line except the last.
    if(!queue.empty())
        out << "\n";
}