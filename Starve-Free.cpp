#include <iostream>
using namespace std;

struct Node
{
    Node* next;
    pid_t value;
};


class Queue
{
    Node* Front, *Rear;
    
    public:
   	void push(pid_t val)
    {
        Node* new_process = new Node();
        new_process->value = val;
       
        if(Rear != NULL)
            Rear->next = new_process;
        else
            Front = new_process;

        Rear = new_process;
    }

    pid_t pop()
    { 

        if(Front == NULL)
        {
            return -1;
        }
        else
        {

            pid_t process_id = Front->value;

            Front = Front->next;
  
            if(Front == NULL)
                Rear = NULL;

            return process_id;
        }
    }
};

class Semaphore
{
    
    int value = 1;

    Queue processes = Queue();
    
    public:
    void wait(pid_t process_id)
    {
        value--;
        if(value < 0)
        {
            processes.push(process_id);
            
            wait(process_id); 
        }
    }
    
    void signal()
    {
        value++;
        if(value <= 0)
        {
            pid_t pid = processes.pop();
            wakeup(pid); 
        }
    }
};

struct Semaphore rw_queue;
struct Semaphore resource;
struct Semaphore r_mutex;

int reader_count = 0;

int data = 1;

void *writer(pid_t process_id){
    
    rw_queue.wait(process_id);
    resource.wait(process_id);
    rw_queue.signal();
    
    data = data*2;

    resource.signal();
}

void *reader(pid_t process_id){
    
    rw_queue.wait(process_id);
    r_mutex.wait(process_id);

    reader_count++;

    if(reader_count == 1)
        resource.wait(process_id);

    rw_queue.signal();
    r_mutex.signal();
    
    printf("%d",data);
    
    r_mutex.wait(process_id);
    reader_count--;

    if(reader_count == 0) 
        resource.signal();

    r_mutex.signal();
}
