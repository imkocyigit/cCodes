
#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <string.h>


#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define POSIX_SOURCE 1
#define TRUE 1
#define FALSE 0

struct termios options;
void openport(void);
void readport(void);

void writeport(void);
int fd;


int main () {
	char buf[50];
	char m;
	char mbf[2];

	//while (1) {
      
		openport();
		printf(" written file  WILL BE WRITTEN\n");
		//int w1=write(fd,"\n written file\0",16);
		
		printf("enter the command : ");

		scanf("%c",&m);
		mbf[0]=m;
		mbf[1]='\n';

		int w=write(fd,mbf,2);
		//write(fd,"\0",1);
		if (w<0) {
			fprintf(stderr,"error = %s\n",strerror(errno));
			exit(-1);
		}

       	 	printf("\n%d chars written\n",w);
		

        
		//lseek(fd,0,SEEK_SET);
	
		int r1=read(fd,buf,49);

        	printf("\n%d chars read\n",r1);
        	printf("read buf: %s",buf);
	
        	buf[49]='\0';
		if (r1!=16) {
                        fprintf(stderr,"error = %s\n",strerror(errno));
                        exit(-1);
                }
		
		

		int c=close(fd);

		if (c==-1) {
                        fprintf(stderr,"error = %s\n",strerror(errno));
                        exit(-1);
                }



		//close(fd);
	//}
}


void openport(void) {
	//device is opened with specific parameters.
	fd=open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
	//opening is corrected.
	if(fd < 0) perror(MODEMDEVICE);
	else printf("\nport opened successfully");
	//input control adjusted
	fcntl(fd,F_SETFL,0/* FNDELAY*/);
	tcgetattr(fd,&options);
	
	if (fd < 0) printf("error");

	//bzero(&options,sizeof(options));

	//control options
	
	options.c_cflag = BAUDRATE |/* CRTSCTS |*/ CS8 | CLOCAL | CREAD;

	//input options
	
	options.c_iflag = IGNPAR | ICRNL;

	//output options
	
	options.c_oflag =0;

	//local options 
	
	options.c_lflag =ICANON;
	//Control Characters
        options.c_cc[VINTR]    = 0;     /* Ctrl-c */
        options.c_cc[VQUIT]    = 0;     /* Ctrl-z */
        options.c_cc[VERASE]   = 0;     /* BACKSPACE */
        options.c_cc[VKILL]    = 0;     /* Ctrl-U */

        options.c_cc[VEOF]     = 0;     /* Ctrl-d */
        options.c_cc[VTIME]    = 0;     /* inter-character timer unused */
        options.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
        options.c_cc[VSWTC]    = 0;     /* '\0' */
        options.c_cc[VSTART]   = 0;     /* Ctrl-q */
        options.c_cc[VSTOP]    = 0;     /* Ctrl-s */
        options.c_cc[VSUSP]    = 0;     /* Ctrl-z */
        options.c_cc[VEOL]     = 0;     /* '\0' */
        options.c_cc[VREPRINT] = 0;     /* Ctrl-r */
        options.c_cc[VDISCARD] = 0;     /* Ctrl-u */
        options.c_cc[VWERASE]  = 0;     /* Ctrl-w */
        options.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
        options.c_cc[VEOL2]    = 0;     /* '\0' */
}

/*void readport(void) {
	char buff=0;
	
	read(fd,&buff,1);
	printf("%c",buff);
	
}

void writeport(void) {
	char cmd=0;
	printf("\n enter the command :");
	//scanf("%c",&cmd);
	int a=write(fd,"f",1);

	lseek(fd,0,SEEK_SET);
	read(fd,&cmd,1);
	printf("%c",cmd);
	printf("\n %d chars written\n",a);
}*/
