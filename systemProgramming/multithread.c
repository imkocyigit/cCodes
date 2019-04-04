#include <pthread.h>
#include <stdio.h>

int data = 0;
pthread_mutex_t lock,lock2;
/* This is our thread function.  It is like main(), but for a thread*/
void *threadFunc(void *arg)
{
	char *str;
	int i = 0;
	printf("first thread id is : %d\n",pthread_self());
	str=(char*)arg;

	while(i < 110 )
	{	
		pthread_mutex_lock(&lock);
		data++;
	
		printf("%d First thread says: %s\n",data,str);
		++i;
		pthread_mutex_unlock(&lock);
		usleep(1);
	}

	return NULL;
}

void *threadFunc1(void *arg)
{	
	char *str;
	int i = 0;

	printf("second thread id is : %d\n",pthread_self());
	str=(char*)arg;

	while(i < 110 )
	{	
		pthread_mutex_lock(&lock);
		data++;
		
		printf("%d second thread says: %s\n",data,str);
		++i;
		pthread_mutex_unlock(&lock);
		usleep(1);
	}

	return NULL;
}


int main(void)
{
	if(pthread_mutex_init(&lock2,NULL) && pthread_mutex_init(&lock,NULL)!= 0){
		printf("\n mutex init failed.\n");
		return 1;
	}



	pthread_t pth,pth2;	// this is our thread identifier
	int i = 0;

	printf("main thread id is : %d\n",pthread_self());
	pthread_create(&pth,NULL,threadFunc,"SA");
	pthread_create(&pth2,NULL,threadFunc1,"AS");
/*	while(i < 100)
*	{	
*		data++;
*		usleep(1);
*		printf("%d  main is running...\n",data);
*		++i;
*	}
*/
	printf("main waiting for thread to terminate...\n");
	pthread_join(pth,NULL);
	pthread_join(pth2,NULL);
	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&lock2);


	return 0;
}
