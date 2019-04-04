
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

struct rets {

	int freqint,mfi1,mff1,mfi2,mff2;
	int *adc;
	int *fdev;
	int *sdev;
	char *message;
};








struct termios options;
int fd;


void openport(void) {
	//device is opened with specific parameters.
	fd=open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
	//opening is corrected.
	if(fd < 0) perror(MODEMDEVICE);
	else printf("\nport opened successfully\n");
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
	
	options.c_lflag |=ICANON;
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

char * readport(void) {
	char buff=0;
	static char datset[1100];
	char *ds;
	int cnt=0;
	ds=datset;
	while (1) {
		read(fd,&buff,1);
		
		if( buff == '~' ) break;
		*ds=buff;
		ds++;
		cnt++;
		//printf("%c",buff);
	}
	//printf("saved values : \n");
	//printf("%s%d",datset,cnt);
	return datset;
}


void confport(void) {
	char cnf[]="stty -F /dev/ttyUSB0 min 1 time 5 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke";

	char dispcnf[]="stty -F /dev/ttyUSB0";

	system(cnf);
	system(dispcnf);

}

struct rets* ikea(char *p) {
	struct rets *retvals=(struct rets*)malloc(sizeof(struct rets));
	retvals->adc=(int *)malloc(50*sizeof(int));
	retvals->fdev=(int *)malloc(49*sizeof(int));
	retvals->sdev=(int *)malloc(48*sizeof(int));
	char *dsp=p;
	char freq[5];
	char temp[5];
	int adc[50];
	int fdev[49];
	int sdev[48];
	int freqint;
	int mfi1;
	int mff1;
	int mfi2;
	int mff2;
	int i,j;
	for(i=0;i<5;i++) {
		freq[i]=*dsp;
		dsp++;
	}
	freqint=atoi(freq);
	retvals->freqint=freqint;
	printf("\nFrequency: %d",freqint);
	printf("\nADC VALUES :");

	for(i=0;i<50;i++) {

		for(j=0;j<5;j++) {
		temp[j]=*dsp;
		dsp++;
		}
	adc[i]=atoi(temp);

	retvals->adc[i]=adc[i];
	printf("%d ",adc[i]);
	}
	printf("\nFist Derivate VALUES :");

	for(i=0;i<49;i++) {

		for(j=0;j<5;j++) {
		temp[j]=*dsp;
		dsp++;
		}
	fdev[i]=atoi(temp);
	printf("%d ",fdev[i]);
	
	retvals->fdev[i]=fdev[i];
	}
	printf("\nSecond Derivate VALUES :");

	for(i=0;i<48;i++) {

		for(j=0;j<5;j++) {
		temp[j]=*dsp;
		dsp++;
		}
	sdev[i]=atoi(temp);
	printf("%d ",sdev[i]);

	retvals->sdev[i]=sdev[i];	
	}

	for(i=0;i<5;i++) {
		temp[i]=*dsp;
		dsp++;
	}
	mfi1=atoi(temp);
	retvals->mfi1=mfi1;


	for(i=0;i<5;i++) {
		temp[i]=*dsp;
		dsp++;
	}
	mff1=atoi(temp);
	retvals->mff1=mff1;	
	printf("\nIn first derivative,%d repeated %d times.\n",mfi1,mff1);

	
	for(i=0;i<5;i++) {
		temp[i]=*dsp;
		dsp++;
	}
	mfi2=atoi(temp);
	retvals->mfi2=mfi2;
	for(i=0;i<5;i++) {
		temp[i]=*dsp;
		dsp++;
	}
	mff2=atoi(temp);
	retvals->mff2=mff2;
	printf("In second derivative,%d repeated %d times.\n",mfi2,mff2);
	retvals->message=dsp;
	printf("\n%s",dsp);
	return retvals;
}
