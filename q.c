#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#define PSWD_LEN 3                                                      
#define QUEUE_LENGTH 16                                                 
                       
 
typedef char pswd_t[PSWD_LEN+1];                                        
 
typedef struct  {                                                
        pswd_t pswd;                                                   
        char * hash;                                                    
} task_t;
 
typedef struct  {                                              
        task_t * tasks[QUEUE_LENGTH];                                   
        int tail;                                                      
        int head;                                                       
        pthread_mutex_t tail_mutex;                                     
        pthread_mutex_t head_mutex;                                     
        sem_t full_sem;                                                 
        sem_t empty_sem;                                           
} queue_t;
 
			
void queue_init(queue_t * queue)  {
	queue->tail=0;
	queue->head=0;
	sem_init(&(queue->full_sem),0,sizeof(queue->tasks)/sizeof(queue->tasks[0])); //размер массива
	sem_init(&(queue->empty_sem),0,0);
	pthread_mutex_init(&(queue->tail_mutex), NULL);
	pthread_mutex_init(&(queue->head_mutex), NULL);		
}

void queue_push(queue_t * queue, task_t * task)  {
	sem_wait(&(queue->full_sem)); //уменьшаем на 1
	pthread_mutex_lock(&(queue->tail_mutex));
	queue->tasks[queue->tail]=task;                              
        queue->tail++;  
	if (queue->tail==sizeof(queue->tasks)/sizeof(queue->tasks[0]))  {
		queue->tail=0;
	}
	pthread_mutex_unlock(&(queue->tail_mutex));                     
        sem_post(&(queue->empty_sem));//увеличиваем на 1                             
}  

task_t * queue_pop(queue_t * queue)  {
	task_t * task;
	sem_wait(&(queue->empty_sem)); //уменьшаем на 1
	pthread_mutex_lock(&(queue->head_mutex));
	task=queue->tasks[queue->head];                              
        queue->head++;  
	if (queue->head==sizeof(queue->tasks)/sizeof(queue->tasks[0]))  	{
		queue->head=0;
	}
	pthread_mutex_unlock(&(queue->head_mutex));                     
        sem_post(&(queue->full_sem));//увеличиваем на 1   
	return task;
}


int main(int argc, char *argv[])  {

        queue_t queue;                                                  
        queue_init(&queue);                                             
        task_t task={
		.pswd="AAA",
		.hash=""
	};
	queue_push(&queue,&task);
	task_t * new_task;                                             
        new_task = queue_pop(&queue);                                   
        printf("%s\n", new_task->pswd);    
	return 0;                            
}
