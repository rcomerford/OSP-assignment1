/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include "reader.h"

// initialise static variables
ofstream writer::out;
deque<string> writer::queue;

void writer::init(const std::string& file_name)
{
    cout << "Writers initialised with file name: " << file_name << endl;
    out.open(file_name.c_str(), ios::out);
}

void writer::run()
{
    // create new thread
    pthread_t pt_id;
    pthread_create(&pt_id, NULL, &runner, NULL);
    cout << "Created writer thread with ID: " << pt_id << endl;
}

// TODO implement
void* writer::runner(void* arg)
{
    while(!queue.empty())
    {
        string next_string = queue.front();
        out << next_string;
        queue.pop_front();  

        // output endline char for every line except the last.
        if(!queue.empty())
        {
            out << endl;
        }
    }

    return nullptr;
}

void writer::append(const std::string& line)
{
    cout << "Appended line" << endl;
    queue.push_back(line);  
}

// TODO implement
void writer::setFinished()
{
    // what is this fn for??
}