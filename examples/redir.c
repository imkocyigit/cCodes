#include <stdio.h>
#include <stdlib.h>

int main (void) {


	FILE *fp;

	printf("This will display on screen \n");

	if((fp=freopen("/dev/pts/2","w", stdout))==NULL) {

		printf("cannot open file \n");
		exit(1);
	}

	printf("this will be written to the file OUT");

	fclose(fp);
}

