//a simple program for Linked lists
struct Node {
	int data;
	struct Node *next;
};

void deleteNodePosition(struct Node **head_ref, int position) {
   // If linked list is empty
   if (*head_ref == NULL)
      return;

   // Store head node
   struct Node* temp = *head_ref;

    // If head needs to be removed
    if (position == 0) {
        *head_ref = temp->next;   // Change head
        free(temp);               // free old head
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
