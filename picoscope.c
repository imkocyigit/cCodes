#define _GNU_SOURCE

#include <pthread.h>
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
void ikea(char *p);
char * readport(void);
void confport(void);
void writeport(void);
int fd;



struct args {

	char *startpoint;
	int upperfor;
	int *retint;
};


int main () {
	char buf[50];
	char *m;
	char mbf[2];

	//while (1) {
      

		
		confport();
		openport();	

		m=readport();
		ikea(m);
		printf("\n");

		int c=close(fd);

		if (c==-1) {
                        fprintf(stderr,"error = %s\n",strerror(errno));
                       exit(-1);
                }
}


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

void *thr(void *a) {

	char *sp;
	int uf;
	int *ri;
	char temp[5];
	int i;

	struct args p;
	p=*((struct args *)a);

	sp = p.startpoint;
	uf = p.upperfor;
	ri = p.retint;

	for(i=0;i<uf;i++) {

		for(j=0;j<5;j++) {
		temp[j]=*sp;
		sp++;
		}

	ri=atoi(temp);
	pthread_exit(&ri);


}




void ikea(char *p) {
	char *dsp=p;
	
	int adc[50];
	int fdev[49];
	int sdev[48];
	int *freqint,*mfi1,*mff1,*mfi2,*mff2;
	
	pthread_t freqthr,adcthr,fdthr,sdthr,mfi1thr,mfi2thr,mff1thr,mff2thr;
	

	struct args *adcargs;
	adcargs=malloc(sizeof(struct args));
	adcargs->startpoint=dsp+5;
	adcargs->upperfor=50;
	
	struct args *fdargs;
	fdargs=malloc(sizeof(struct args));
	fdargs->startpoint=dsp+255;
	fdargs->upperfor=49;


	struct args *sdargs;
	sdargs=malloc(sizeof(struct args));
	sdargs->startpoint=dsp+500;
	sdargs->upperfor=48;


	struct args *mfi1args;
	mfi1args=malloc(sizeof(struct args));
	mfi1args->startpoint=dsp+740;
	mfi1args->upperfor=1;


	struct args *mff1args;
	mff1args=malloc(sizeof(struct args));
	mff1args->startpoint=dsp+745;
	mff1args->upperfor=1;


	struct args *mfi2args;
	mfi2args=malloc(sizeof(struct args));
	mfi2args->startpoint=dsp+750;
	mfi2args->upperfor=1;


	struct args *mff2args;
	mff2args=malloc(sizeof(struct args));
	mff2args->startpoint=dsp+755;
	mff2args->upperfor=1;

	struct args *freqargs;
	freqargs=malloc(sizeof(struct args));
	freqargs->startpoint=dsp;
	freqargs->upperfor=1;
	
	//pthread_t freqthr,adcthr,fdthr,sdthr,mfi1thr,mfi2thr,mff1thr,mff2thr;
	pthread_create(&freqthr,NULL,thr,(void *)freqargs);
	pthread_create(&adcthr,NULL,thr,(void *)adcargs);
	pthread_create(&fdthr,NULL,thr,(void *)fdargs);
	pthread_create(&sdthr,NULL,thr,(void *)sdargs);
	pthread_create(&mfi1thr,NULL,thr,(void *)mfi1args);
	pthread_create(&mff1thr,NULL,thr,(void *)mff1args);
	pthread_create(&mfi1thr,NULL,thr,(void *)mfi1args);
	pthread_create(&mff2thr,NULL,thr,(void *)mff2args);





}
