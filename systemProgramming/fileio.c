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
	int fd1;
	char buff[17];
	int sz;

	fd1=open("dnm.txt",O_RDWR | O_NDELAY | O_NOCTTY);
	fcntl(fd1,F_SETFL,0);
	

	if (fd1 == -1) printf("Failed to create and open the file \n");

	int w1=write(fd1,"\n written file \n",16);
	lseek(fd1,0,SEEK_SET);

	printf("\n%d chars written\n ",w1);


	int r1=read(fd1,buff,16);
	buff[16]='\0';
	
	printf("buf: %s\n",buff);
	
	printf("\n%d chars read\n ",r1);
	lseek(fd1,9,SEEK_SET);

	int w2=write(fd1," mete ",6);

	printf("\n%d chars written\n ",w2);

	lseek(fd1,0,SEEK_SET);
	int r2=read(fd1,buff,16);
	buff[16]='\0';

	printf("\n%d chars read\n ",r2);

	
	printf("buf2: %s\n",buff);
	close(fd1);

	openport();
	readport();
	lseek(fd,0,SEEK_SET);
	//close(fd);
	//openport();
	writeport();
	lseek(fd,0,SEEK_SET);
	//close(fd);
	//openport();
	readport();
	close(fd);
}


void openport(void) {
	//device is opened with specific parameters.
	fd=open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY );
	//opening is corrected.
	if(fd < 0) perror(MODEMDEVICE);
	//input control adjusted
	fcntl(fd,F_SETFL,0);
	tcgetattr(fd,&options);
	
	if (fd < 0) printf("error");

	//bzero(&options,sizeof(options));

	//control options
	
	options.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

	//input options
	
	options.c_iflag = IGNPAR | ICRNL;

	//output options
	
	options.c_oflag =0;

	//local options 
	
	options.c_lflag = ICANON;
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

void readport(void) {
	char buff=0;
	
	while (1) {
		read(fd,&buff,1);
		
		if( buff == 'x' ) break;
		printf("%c",buff);
	}


}

void writeport(void) {
	char cmd=0;
	printf("\n enter the command :");
       	//scanf("%c",&cmd);

	//printf("%c",cmd);
	cmd=0b01100110;
	int a=write(fd,"f",1);
	printf("%d",a);
}

