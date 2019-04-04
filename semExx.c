// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <math.h>
#include <stdlib.h>
#include <limits.h>

struct Node {
	int data;
	struct Node *next;
};




// This function prints contents of linked list starting from  
// the given node 

int justpop(struct Node** head_ref);
void deleteList(struct Node** head_ref);
void printList(struct Node *n); 
void push(struct Node** head_ref, int new_data); 
void insertAfter(struct Node* prev_node, int new_data); 
void append(struct Node** head_ref, int new_data); 
void deleteNode(struct Node **head_ref, int key);
int findLengthiter(struct Node** head_ref);
int getCount(struct Node * head);
void deleteNodePosition(struct Node **head_ref, int position);

sem_t emptycount,fillcount,ready,mutex; 


void* producer(void* arg) { 
	//wait 
	
	struct Node* node = (struct Node *)arg;

	while(1) {	
       	int rast=rand()%100;
	sem_wait(&emptycount); 
	sem_wait(&mutex);
	//critical section 
	
	printList(node); 
	printf("\nEntered Producer1..\n");
	printf("%d is written to the buffer\n",rast);

	append(&node,rast);

	printf("Just Exiting Producer...\n"); 
	printList(node); 
	sleep(1);
	//ends critical section
	sem_post(&mutex);
	sem_post(&fillcount);


       	usleep(1);	
	}
} 

void* producer2(void* arg) { 
	//wait 
	
	struct Node* node = (struct Node *)arg;


	while(1) {	
       	int rast=rand()%100+200;
	sem_wait(&emptycount); 
	sem_wait(&mutex);
	//critical section 
	
	printList(node); 
	printf("\nEntered Producer2..\n");
	printf("%d is written to the buffer\n",rast);
	append(&node,rast);
	printf("Just Exiting Producer2...\n"); 
	printList(node); 
	sleep(1);
	//ends critical section
	sem_post(&mutex);
	sem_post(&fillcount);
       	usleep(1);	
	}
}

void* consumer(void* arg) { 
	//wait 

	//sleep(1);

	struct Node* node = (struct Node *)arg;
	while(1) {	
	sem_wait(&fillcount); 
	sem_wait(&mutex);
	int a=node->data;
	//critical section 
	printf("\t\t\t\t \t\t\t\tEntered Consumer1..\n");
	printf("\t\t\t\t \t\t\t\t1---%d is read from the buffer\n",a);
	printf("\t\t\t\t \t\t\t\tJust Exiting Consumer...\n"); 

	//ends critical section
	sem_post(&mutex);
	sem_post(&emptycount);
	sem_post(&ready);
	//sem_wait(&ready);
	//sem_wait(&ready);
	deleteNodePosition(&node,0);
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<NODE DELETED>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
       	usleep(1);	
	}

}


void* consumer2(void* arg) { 
	//wait 

	//sleep(1);
	struct Node* node = (struct Node *)arg;
	while(1) {	
	sem_wait(&fillcount); 
	sem_wait(&mutex);

	int a=(node->next)->data;
	//critical section 
	printf("\t\t\t\t \t\t\t\tEntered Consumer2..\n");
	printf("\t\t\t\t \t\t\t\t2---%d is read from the buffer\n",a);
	printf("\t\t\t\t \t\t\t\tJust Exiting Consumer...\n"); 

	//ends critical section
	sem_post(&mutex);
	sem_post(&emptycount);
	sem_post(&ready);
	//sem_wait(&ready);
	//sem_wait(&ready);
	//deleteNodePosition(&node,0);
	//printf("\n\n<<<<<<<<<<first node of list is deleted>>>>>>>>>>\n\n");
	
       	usleep(1);
	}	

} 

int main() 
{ 
	
	struct Node* head = (struct Node *) malloc(sizeof(struct Node));
	sem_init(&ready,0,-2);
	sem_init(&emptycount,0,10);
	sem_init(&fillcount,0,0);
	sem_init(&mutex, 0, 1); 
	pthread_t t1,t2,t3,t4; 
	pthread_create(&t1,NULL,producer,(void *)head); 
	pthread_create(&t2,NULL,consumer,(void *)head); 
	pthread_create(&t1,NULL,producer2,(void *)head); 
	pthread_create(&t2,NULL,consumer2,(void *)head);


	pthread_join(t1,NULL); 
	pthread_join(t2,NULL);
	pthread_join(t3,NULL); 
	pthread_join(t4,NULL);

	sem_destroy(&emptycount); 
	sem_destroy(&fillcount); 
	sem_destroy(&ready); 
	sem_destroy(&mutex); 
	return 0; 
} 

void deleteNodePosition(struct Node **head_ref, int position) {
   // If linked list is empty
   if (*head_ref == NULL)
      return;

   // Store head node
   struct Node* temp = *head_ref;

    // If head needs to be removed
    if (position == 0) {
	free(*head_ref);
        *head_ref = temp->next;   // Change head
        //free(temp);               // free old head
        return;
    }

    // Find previous node of the node to be deleted
    for (int i=0; temp!=NULL && i<position-1; i++)
         temp = temp->next;

    // If position is more than number of ndoes
    if (temp == NULL || temp->next == NULL)
         return;

    // Node temp->next is the node to be deleted
    // Store pointer to the next of node to be deleted
    struct Node *next = temp->next->next;

    // Unlink the node from linked list
    free(temp->next);  // Free memory

    temp->next = next;  // Unlink the deleted node from list
}


void printList(struct Node *n) { 
	printf("\nCURRENT LIST :");
  while (n != NULL)  { 
     printf(" %d ", n->data); 
     n = n->next; 
  } 
  printf("\n");
} 


void push(struct Node** head_ref, int new_data) { 
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node)); 
  
    /* 2. put in the data  */
    new_node->data  = new_data; 
  
    /* 3. Make next of new node as head */
    new_node->next = (*head_ref); 
  
    /* 4. move the head to point to the new node */
    (*head_ref)    = new_node; 
}

int justpop(struct Node** head_ref) {
	return (*head_ref) -> data;	
}

  
/* Given a node prev_node, insert a new node after the given  
   prev_node */
void insertAfter(struct Node* prev_node, int new_data) { 
   /*1. check if the given prev_node is NULL */
    if (prev_node == NULL) 
    { 
      printf("the given previous node cannot be NULL"); 
      return; 
    } 
  
    /* 2. allocate new node */
    struct Node* new_node =(struct Node*) malloc(sizeof(struct Node)); 
  
    /* 3. put in the data  */
    new_node->data  = new_data; 
  
    /* 4. Make next of new node as next of prev_node */
    new_node->next = prev_node->next; 
  
    /* 5. move the next of prev_node as new_node */
    prev_node->next = new_node; 
} 


  
/* Given a reference (pointer to pointer) to the head 
   of a list and an int, appends a new node at the end  */
void append(struct Node** head_ref, int new_data) { 
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node)); 


    struct Node *last = *head_ref;  /* used in step 5*/
  
    /* 2. put in the data  */
    new_node->data  = new_data; 
 

    /* 3. This new node is going to be the last node, so make next of 
          it as NULL*/
    new_node->next = NULL; 
 

    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL) { 
       *head_ref = new_node; 
       return; 
    } 
  

    /* 5. Else traverse till the last node */
    while (last->next != NULL) 
        last = last->next; 
  

    /* 6. Change the next of last node */
    last->next = new_node; 
    return; 
} 


/* Given a reference (pointer to pointer) to the head of a list
   and a key, deletes the first occurrence of key in linked list */


void deleteNode(struct Node **head_ref, int key) {
   
    // Store head node
    struct Node* temp = *head_ref, *prev;


    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->data == key) {
        *head_ref = temp->next;   // Changed head
        free(temp);               // free old head
        return;
    }


    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }


    // If key was not present in linked list
    if (temp == NULL) return;


    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp);  // Free memory
}


/* Function to delete the entire linked list */
void deleteList(struct Node** head_ref) { 
   /* deref head_ref to get the real head */
   struct Node* current = *head_ref; 
   struct Node* next; 
  
   while (current != NULL) { 
       next = current->next; 
       free(current); 
       current = next; 
   } 
    
   /* deref head_ref to affect the real head back 
      in the caller. */
   *head_ref = NULL; 
}   


int findLengthiter(struct Node** head_ref) {
	int cnt = 0;
	struct Node *current=*head_ref;
	while(current!=NULL) {
		current = current->next;
		cnt++;
	}
	return cnt;
}


int getCount(struct Node * head) {
	if(head == NULL) return 0;
	return 1+getCount(head->next);
}





