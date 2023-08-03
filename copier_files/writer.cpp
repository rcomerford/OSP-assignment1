/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"

writer::writer(const string& file_name)
{
    cout << "Writer created with file name: " << file_name << endl;
    
    out.open(file_name); 
    // TODO fix so that it writes to any system path
    // current behaviour means no writing is done is
    // file name is not in the current directory.
}

void writer::run()
{
    while(!queue.empty())
        writeNextLine();
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
}