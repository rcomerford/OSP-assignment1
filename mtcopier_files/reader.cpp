/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "reader.h"
#include "writer.h"

// initialise static variables
ifstream reader::in;
pthread_mutex_t reader::read_lock;

void reader::init(const string& file_name)
{
    cout << "Readers initialised with file name: " << file_name << endl;
    in.open(file_name.c_str());
}

pthread_t reader::run()
{
    // create new thread
    pthread_t pt_id;
    cout << "Created reader thread with ID: " << pt_id << endl;
    pthread_create(&pt_id, NULL, runner, NULL);
    return pt_id;
}

void* reader::runner(void* arg)
{
    string current_line;

    while(read(current_line))
    {
        // add each line to the writer's shared queue
        writer::append(current_line);  
    }

    cout << "done reader!\n";
    return nullptr;
}

bool reader::read(string& line)
{
    bool success = true;
    pthread_mutex_lock(&read_lock);  

    if(!getline(in, line))
        success = false;
    // else
        // cout << "Read: " << line << '\n';

    pthread_mutex_unlock(&read_lock);  
    return success;
}