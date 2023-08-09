/**
 * Startup code provided by Paul Miller for Operating Systems Principles
*/

#include "writer.h"
#include "reader.h"

// initialise static variables
ofstream writer::out;
deque<string> writer::queue;

pthread_mutex_t writer::queue_lock;

pthread_mutex_t writer::append_lock;
pthread_mutex_t writer::pop_lock;

pthread_mutex_t writer::write_lock;


pthread_cond_t writer::queue_not_empty;

void writer::init(const std::string& file_name)
{
    cout << "Writers initialised with file name: " << file_name << endl;
    out.open(file_name.c_str(), ios::out);

    // create mutex
    pthread_mutex_init(&queue_lock, nullptr);
    pthread_mutex_init(&write_lock, nullptr);

    // pthread_cond_init() 
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
    for(int i = 0; i < 10000; ++i)
    {
        string next_string = writer::pop();
        writer::write(next_string);
    }

    cout << "done writer!\n";
    return nullptr;
}

void writer::append(const string& line)
{
    pthread_mutex_lock(&append_lock);
    pthread_mutex_lock(&queue_lock);

    queue.push_back(line);
    // cout << "new queue size: " << queue.size() << '\n';
    // cout << "queue front: " << queue.front() << '\n';
    // queue.pop_front();
    // cout << "queue front: " << queue.front() << '\n';
    // cout << "new queue back: " << queue.back() << '\n';

    pthread_mutex_unlock(&queue_lock);
    pthread_mutex_unlock(&append_lock); 
}


// CHANGE TO BE LIKE READ WITH THE REFERENCE THANKS I LOVE YOU BYE
string writer::pop()
{
    // pthread_cond_wait(&queue_not_empty, nullptr);


    pthread_mutex_lock(&pop_lock);
    pthread_mutex_lock(&queue_lock);

    string next_string = "";

    if(!queue.empty())
    {
        next_string = queue.front();
        queue.pop_front();
    }

    // cout << "Popped line: " << std::to_string(queue.size()) << endl;

    pthread_mutex_unlock(&queue_lock);
    pthread_mutex_unlock(&pop_lock);  

    return next_string;
}

void writer::write(string& line)
{
    pthread_mutex_lock(&write_lock);  

    if(line.length() > 0)
    {
        out << line << '\n';
        cout << "Written: " << line << '\n';
    }
    
    pthread_mutex_unlock(&write_lock);
}