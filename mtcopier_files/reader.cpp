/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"

// initialise static variables
ifstream reader::in;

void reader::init(const string& file_name)
{
    cout << "Readers initialised with file name: " << file_name << endl;
    in.open(file_name.c_str(), ios::in);
}

void reader::run()
{
    // create new thread
    pthread_t pt_id;
    pthread_create(&pt_id, NULL, &runner, NULL);
    cout << "Created reader thread with ID: " << pt_id << endl;
}

void* reader::runner(void* arg)
{
    string current_line;

    // read line from shared input stream
    while (getline(in, current_line)) 
        // add each line to the writer's shared queue
        writer::append(current_line);

    return nullptr;
}