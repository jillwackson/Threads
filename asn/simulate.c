/******************************************************************************
*
******************************************************************************/

#include "simulate.h"

int memory, max_memory;
FILE *fp;
linked_stack_t *jobs;

void simulate(int memory_value, linked_stack_t *stack)
{
    //memory setup
    memory = memory_value;
    max_memory = memory_value;
    //create an array of threads equal in size to the number of theads that we will allow to run concurrently.
    pthread_t threads[NUMBER_OF_THREADS];
    int memory_usages[NUMBER_OF_THREADS];
    //counter for how many threads we have running 
    int i = 0;
    //get the current job and then break it into its components
    job_t *current_job = pop(stack);
    int current_job_number = current_job->number;
    int current_job_memory = current_job->required_memory;
    int current_job_time = current_job->required_time;
    //while there are still jobs on the stack to be executed
    while (current_job!=NULL){
        //not enough memory total
        if (current_job_memory > max_memory){
            print_exceed_memory(fp, current_job_number);
        }
        //not enough memory right now
        else if (memory-current_job_memory<0){
            print_insufficient_memory(fp, current_job_number);
            //push the job back onto the stack for later
            push(stack, current_job);
        }
        //check to make sure we are not running too many threads
        else if (i < NUMBER_OF_THREADS){
            //allocate memory
            memory = memory - current_job_memory;
            memory_usages[i] = current_job_memory;
            print_allocate_memory(fp, memory, current_job_memory);
            //start the thread
            //pack args
            int args[2] = {current_job_time, current_job_number};
            if(pthread_create( &threads[i++], NULL, simulate_thread, (void*) current_job){
                perror("Thread Issue");
            }else{
                
            }
        }
        //all of our threads are being used, lets wait for them to be free'd up.
        else if (i >= NUMBER_OF_THREADS){
            while (i>0){
                //deallocate memory and join threads
                memory = memory + memory_usages[i];
                print_deallocate_memory(fp, memory ,memory_usages[i]);
                memory_usages[i] = 0;
                pthread_join(threads[i--], NULL); 
            }
        }
        

    }
}

//simulates the thread
void *simulate_thread(void * ptr){
    job_t *job;
    job = (job_t *) ptr;
    print_starting(fp, job->number);
    sleep(sleep_time);
    print_completed(fp, job->number);
}