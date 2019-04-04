#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"


void main (int argc, char *argv[])	{
	int disp,count;

	if(argc<2)	{
		printf("You must enter the length of the count \n");
		printf("ont the command line. try again. \n");
		exit(1);

	}

	if(argc==3 && !strcmp(argv[2],"display")) disp=1;
	else disp=0;

	for(count=atoi(argv[1]); count; count--) if(disp=1) printf("%d\n",count);

	putchar(7); //this will ring the bell

	printf("done");
}

