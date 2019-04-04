#include<stdio.h> 
#include <stdlib.h>
struct Point { 
	int x, y; 
}p1,*p; 



void func(void *str) {

	struct Point s;
	s=*((struct Point *)str);

	printf ("x = %d, y = %d\n",s.x, s.y); 
	 


}


int main()  { 
	p1.y=2; 
	p1.x = 20;
	p=malloc(sizeof(struct Point));
	p->x=10;
	p->y=15;

	func((void*)p);

	return 0; 
}

