/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"

reader::reader(const string& file_name, writer& my_writer) : the_writer(my_writer)
{
    cout << "Reader created with file name: " << file_name << endl;
    in.open(file_name.c_str(), std::ios::in);
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