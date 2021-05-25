It is also called as **Third Readers-Writers** problem. It requires that no thread shall be allowed to starve; that is, 
the operation of obtaining a lock on the shared data will always terminate in a bounded amount of time.

**Problem**:- A Writer thread does not have a chance to execute while any number of 
Readers continuously entering and leaving the working area.

To avoid this problem the following is ***pseudocode*** of commonly known solution is proposed.

```
//creating semaphore

struct Semaphore{
  int value = 1;
  FIFO_Queue* Q = new FIFO_Queue();
}
    
void wait(Semaphore *S,int* process_id){
  S->value--;
  if(S->value < 0){
  S->Q->push(process_id);
  block(); 
 }
}
    
void signal(Semaphore *S){
  S->value++;
  if(S->value <= 0){
  int* PID = S->Q->pop();
  wakeup(PID);
  }
}

struct FIFO_Queue{
    ProcessBlock* front, rear;
    int* pop(){
        if(front == NULL){
            return -1;         
        }
        else{
            int* val = front->value;
            front = front->next;
            if(front == NULL)
            {
                rear = NULL;
            }
            return val;
        }
    }
    void* push(int* val){
        ProcessBlock* blk = new ProcessBlock();
        blk->value = val;
        if(rear == NULL){
            front = rear = n;
            
        }
        else{
            rear->next = blk;
            rear = blk;
        }
    }
    
}

struct ProcessBlock{
    ProcessBlock* next;
    int* process_block;
}


//Initialization

int read_count = 0;                     
Semaphore turn = new Semaphore();        
                                         
Semaphore rwt = new Semaphore();       
Semaphore r_mutex = new Semaphore();   


//Reader

do{

wait(turn,process_id);              
       wait(r_mutex,process_id);          
       read_count++;                       
       if(read_count==1)                   
         wait(rwt);                       
       signal(turn);                       
                                        
       signal(r_mutex);                  

       wait(r_mutex,process_id)                  
       read_count--;                     
       if(read_count==0)                  
        signal(rwt);                     
       signal(r_mutex);  
       
}while(true);


//Writer

do{

      wait(turn,process_id);              
      wait(rwt,process_id);               
      signal(turn,process_id);           

      signal(rwt)                         

}while(true);
```
