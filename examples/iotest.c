#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>


#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1
#define TRUE 1
#define FALSE 0

FILE *file,*file2;
int fileLen;
char *tmpbuffer;
void openport(void);
void readport(void);
void sendport(void);
int fd=0, n,m;
static int cnt,size, s_cnt;
unsigned char *var;
char sendcmd[10]="\0";


struct termios options;

char buffer[512];

void openport(void) {
        fd=open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY);

        printf("Picoscope %d \n", fd);

        if (fd<0) perror(MODEMDEVICE);

        fcntl(fd,F_SETFL,0);
        tcgetattr(fd,&options);

        bzero(&options,sizeof(options));

        options.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

        options.c_iflag=IGNPAR | ICRNL ;

        options.c_oflag = 0;

        options.c_lflag = ICANON;

        options.c_cc[VINTR]    = 0;     /* Ctrl-c */
        options.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
        options.c_cc[VERASE]   = 0;     /* del */
        options.c_cc[VKILL]    = 0;     /* @ */

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
void main() {

	openport();

	unsigned char buff,buff2;
	file=fopen("input.txt","w+"); //input adinda yeni bir dosya açıldı.

	while (1) {
		
		n=read(fd,&buff,1);//porttan gelen bir char buff'a kaydedildi.

		if(n==-1) switch(errno) {
				case EAGAIN: continue;
				default: goto quit;
				}
			
			if(buff=='x') break;//porttan gelen char 'x' ise okumayı bitir.
			fputc(buff,file);//buff'daki char file'a yazıldı.
			printf("%c",buff);//buff'daki char ekrana basıldı.
		}
	quit:
       		fclose(file);

/*	printf("\nenter the command :");
	scanf("%c",&buff); // komut tek bir char larak buff'a yazıldı.

	n=write(fd,&buff,1);// girilen char'ı komut olarak port'a yaz.

*/	sleep(10);
	
	file2=fopen("anlz.txt","w+"); //analiz için dosya açıldı.

	while (1) {

                m=read(fd,&buff2,1);//porttan gelen bir char buff'a kaydedildi.

                if(m==-1) switch(errno) {
                                case EAGAIN: continue;
                                default: goto quit;
                        }

                        if(buff=='x') break;//porttan gelen char 'x' ise okumayı bitir.
                        fputc(buff2,file2);//buff'daki char file'a yazıldı.
                        printf("%c",buff2);//buff'daki char ekrana basıldı.
                }
        fclose(file);




}


				









