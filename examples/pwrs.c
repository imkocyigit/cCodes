
/*Display powers of the number 1 through 10*/

#include "stdio.h"
#include "stdlib.h"

int pwr(int a, int b);
void table(int p[4][10]);
void show(int p[4][10]);

void main(void)	{

	int *p;

	p=malloc(40*sizeof(int));

	if(!p)	{
		printf("memory request failed\n");
		exit(1);
	}

	table(p);
	show(p);

}

void table (int p[4][10])	{
	register int i,j;
	
	for(j=1;j<11;j++)	
		for(i=1;i<5;i++) p[i][j] = pwr(j,i);
}

void show (int p[4][10])	{
	
	register int i,j;
	
	printf("%10s %10s %10s %10s \n","N","N^2","N^3","N^4");
	
	for(j=1;j<11;j++)	{
		for(i=1;i<5;i++) printf("%10d ",p[i][j]);
		printf("\n");
	}
}

pwr(int a, int b)	{
	register int t=1;
	
	for(; b;b--) t=t*a;
	return t;
}



