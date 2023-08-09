/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include "reader.h"

// initialise static variables
ofstream writer::out;
deque<string> writer::queue;
pthread_mutex_t writer::queue_lock;
pthread_mutex_t writer::write_lock;

void writer::init(const std::string& file_name)
{
    cout << "Writers initialised with file name: " << file_name << endl;
    out.open(file_name.c_str(), ios::out);

    // create mutex
    pthread_mutex_init(&queue_lock, nullptr);
}

pthread_t writer::run()
{
    // create new thread
    pthread_t pt_id;
    cout << "Created writer thread with ID: " << pt_id << endl;
    pthread_create(&pt_id, NULL, &runner, NULL);
    
    return pt_id;
}

void* writer::runner(void* arg)
{
    string next_string = writer::pop();
    writer::write(next_string);

    cout << "done reader!\n";
    return nullptr;
}

void writer::append(const string& line)
{
    pthread_mutex_lock(&queue_lock);

    queue.push_back(line);

    pthread_mutex_unlock(&queue_lock); 
}


// CHANGE TO BE LIKE READ WITH THE REFERENCE THANKS I LOVE YOU BYE
string writer::pop()
{
    pthread_mutex_lock(&queue_lock);

    string next_string = "";

    if(!queue.empty())
    {
        next_string = queue.front();
        queue.pop_front();
    }

    // cout << "Popped line: " << std::to_string(queue.size()) << endl;

    pthread_mutex_unlock(&queue_lock); 

    return next_string;
}

void writer::write(string& line)
{
    pthread_mutex_lock(&write_lock);  

    // line += "\n";

    // line += '\x0a';
    // line += '\n';

    out << line;
    
    pthread_mutex_unlock(&write_lock);
}