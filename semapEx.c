// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <math.h>
#include <stdlib.h>
#include <limits.h>


sem_t emptycount,fillcount,mutex,ready; 
int emptyVal,fillVal,mutexVal,readyVal;
// A structure to represent a queue 
struct Queue { 
	int front, rear, size; 
	unsigned capacity; 
	int* array; 
}; 

// function to create a queue of given capacity. 
// It initializes size of queue as 0 
struct Queue* createQueue(unsigned capacity) { 
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); 
	queue->capacity = capacity; 
	queue->front = queue->size = 0; 
	queue->rear = capacity - 1; // This is important, see the enqueue 
	queue->array = (int*) malloc(queue->capacity * sizeof(int)); 
	return queue; 
} 

// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue) {
       	return (queue->size == queue->capacity);
} 

// Queue is empty when size is 0 
int isEmpty(struct Queue* queue) {
       	return (queue->size == 0);
} 

// Function to add an item to the queue. 
// It changes rear and size 
void enqueue(struct Queue* queue, int item) { 
	if (isFull(queue)) 
		return; 
	queue->rear = (queue->rear + 1)%queue->capacity; 
	queue->array[queue->rear] = item; 
	queue->size = queue->size + 1; 
	//printf("%d enqueued to queue\n", item); 
} 

// Function to remove an item from queue. 
// It changes front and size 
int dequeue(struct Queue* queue) { 
	if (isEmpty(queue)) 
		return INT_MIN; 
	int item = queue->array[queue->front]; 
	return item; 
} 

void delfront(struct Queue* queue) {

	queue->front = (queue->front + 1)%queue->capacity; 
	queue->size = queue->size - 1; 
}
// Function to get front of queue 
int front(struct Queue* queue)	{ 
	if (isEmpty(queue)) 
		return INT_MIN; 
	return queue->array[queue->front]; 
} 

// Function to get rear of queue 
int rear(struct Queue* queue) { 
	if (isEmpty(queue)) 
		return INT_MIN; 
	return queue->array[queue->rear]; 
} 

void* producer(void* arg) { 
	//wait 
	int i;	
	struct Queue* queue = (struct Queue *)arg;

	while(1) {	
	//critical section 
	
	sleep(1);
       	int rast=rand()%100;

	sem_wait(&emptycount); 
	sem_wait(&mutex);
	enqueue(queue,rast);
	printf("%d is written to the buffer by Producer1\n",rast);
	sem_post(&fillcount);
	sem_post(&mutex);

	//ends critical section
       	usleep(1);
	sem_wait(&mutex);
	sem_getvalue(&emptycount,&emptyVal);
	sem_getvalue(&fillcount,&fillVal);
	sem_getvalue(&mutex,&mutexVal);
	sem_getvalue(&ready,&readyVal);

	/*for(i=0;i<100;i++){
	printf("%d ",queue->array[i]);  
	}*/

	printf("\nSTATUS\nempty spaces : %d \nfilled spaces : %d\nmutex value : %d\nready value : %d\n\n",emptyVal,fillVal,mutexVal,readyVal);
	sem_post(&mutex);
	}
} 

void* producer2(void* arg) { 
	//wait 
	
	struct Queue* queue = (struct Queue *)arg;

	while(1) {	
	sleep(1);
	sem_wait(&emptycount); 
	//critical section 
	
       	int rast=rand()%100+200;
	
        sem_wait(&mutex);	
	enqueue(queue,rast);
	printf("%d is written to the bufferby Producer2\n",rast);
	sem_post(&fillcount);
	sem_post(&mutex);

	//ends critical section
       	usleep(1);	
	}
}

void* consumer(void* arg) { 
	//wait 

	struct Queue* queue = (struct Queue *)arg;
	while(1) {	
	sem_wait(&fillcount); 
	//critical section 
	//sleep(1);

	sem_wait(&mutex);
	printf("\t\t\t\t \t\t\t\t%d is read from the buffer by Consumer1\n",dequeue(queue));
	sem_post(&ready);
	sem_post(&emptycount);
	sem_post(&mutex);

	//ends critical section

	sem_wait(&ready);
	sem_wait(&ready);

	sem_wait(&mutex);
	delfront(queue);
	sem_post(&mutex);
       	//usleep(1);	
	}

}


void* consumer2(void* arg) { 
	//wait 

	struct Queue* queue = (struct Queue *)arg;
	while(1) {	
	//critical section 
	//sleep(1);

	sem_wait(&fillcount);
        sem_wait(&mutex);	
	printf("\t\t\t\t \t\t\t\t%d is read from the buffer by Consumer2\n",dequeue(queue));
	sem_post(&ready);
	sem_post(&emptycount);
	sem_post(&mutex);

	//ends critical section
       	//usleep(1);	
	}

} 

int main() 
{ 
	struct Queue* queue = createQueue(100); 
	
	sem_init(&mutex,0,1);
	sem_init(&ready,0,0);
	sem_init(&emptycount,0,100); 
	sem_init(&fillcount, 0,-10); 
	pthread_t t1,t2,t3,t4; 
	pthread_create(&t1,NULL,producer,(void *)queue); 
	pthread_create(&t1,NULL,producer2,(void *)queue); 
	//sleep(3);
	pthread_create(&t2,NULL,consumer,(void *)queue); 
	pthread_create(&t2,NULL,consumer2,(void *)queue);


	pthread_join(t1,NULL); 
	pthread_join(t2,NULL);
	pthread_join(t3,NULL); 
	pthread_join(t4,NULL);

	sem_destroy(&ready); 
	sem_destroy(&mutex); 
	sem_destroy(&emptycount); 
	sem_destroy(&fillcount); 
	return 0; 
} 

