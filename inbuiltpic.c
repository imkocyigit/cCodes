/* 
 * File:   ledy.c
 * Author: mete
 *
 * Created on January 29, 2019, 9:39 AM
 */
#include <pic18f4550.h>
#include <stdio.h>
#include <xc.h>
#include <float.h>
#include <math.h>
#pragma config PLLDIV = 5 , CPUDIV = OSC1_PLL2 , USBDIV = 2

#define _XTAL_FREQ 4000000

//#pragma config FOSC=HS
#pragma config FOSC = XT_XT
//#pragma config FOSC = INTOSCIO_EC
#pragma PWRT = OFF;
#pragma config FCMEN = OFF                                 
#pragma config BORV = 3
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF   

//#define E   LATDbits.LATD2;

/*#define RS  LATDbits.LATD1;
#define RW  LATDbits.LATD0;
#define DB0 LATDbits.LATD3;
#define DB1 LATEbits.LATE2;
#define DB2 LATEbits.LATE1;
#define DB3 LATCbits.LATC6;
#define DB4 LATCbits.LATC7;
#define DB5 LATDbits.LATD4;
#define DB6 LATDbits.LATD5;
#define DB7 LATDbits.LATD6;*/



typedef union  {   
    
    unsigned int data;
    struct  {
    unsigned DB0 :1;
    unsigned DB1 :1;
    unsigned DB2 :1;
    unsigned DB3 :1;
    unsigned DB4 :1;
    unsigned DB5 :1;
    unsigned DB6 :1;
    unsigned DB7 :1;       
    };
}accessiblebyte;


void sample_timer(unsigned int n)   {//1ms'lik delay fonksiyonu.for clock calibration.
    T0CON = 0b10001000; // Prescaler not assigned.                                    
    TMR0 = 65535 - (n/50);  // 30/signal period.
    TMR0ON=1;
    while(!TMR0IF);                                 
    TMR0IF=0;                                       
    TMR0ON=0;
    T0CON = 0x00;                                       
}

void timer_ms(unsigned int n) //1ms'lik delay fonksiyonu.for clock calibration.
{
    unsigned int i;
    T0CON = 0x82;                                       
    for(i=0;i<n;i++) {
        TMR0 = 65535 - (8000000/(4*8*1000));         
        while(!TMR0IF);                                 
        TMR0IF=0;                                       
    }
    T0CON = 0x00;                                       
}


void delayzz()  {
    int i, j;
    for(i=0;i<500;i++) {
        for(j=0;j<2;j++)    {           /* 1ms civar? timer. */           
        }    
    }   
}

void tetikle() { // Enable pinini high to low yaparak tetiklemeyi saglar.
    
    LATDbits.LATD2 =1;
    LATDbits.LATD2 =0;
    delayzz();
    LATDbits.LATD2 =1;
}

void displaySil(){ //ekrani temizler. imleci basa yollar.
    
    LATDbits.LATD1 = 0; // RS
    LATDbits.LATD0 = 0; // RW
    
    LATDbits.LATD6 = 0; // DB7
    LATDbits.LATD5 = 0; // DB6
    LATDbits.LATD4 = 0; // DB5
    LATBbits.LATB1 = 0; // DB4
    
    LATBbits.LATB0 = 0; // DB3
    LATEbits.LATE1 = 0; // DB2
    LATEbits.LATE2 = 0; // DB1
    LATDbits.LATD3 = 1; // DB0

    delayzz();
    tetikle();
}

void dispONOFF(){ // Display acik. imlec kapali ve yanip sonmez.
    
    LATDbits.LATD1 = 0; // RS
    LATDbits.LATD0 = 0; // RW
    
    LATDbits.LATD6 = 0; // DB7
    LATDbits.LATD5 = 0; // DB6
    LATDbits.LATD4 = 0; // DB5
    LATBbits.LATB1 = 0; // DB4
    
    LATBbits.LATB0 = 1; // DB3
    LATEbits.LATE1 = 1; // DB2
    LATEbits.LATE2 = 0; // DB1
    LATDbits.LATD3 = 0; // DB0
    delayzz();
    tetikle();
}

void setFunc(){ // Fonksiyon ayarlari. Satir 1,5*8 matrix , 8 bit comm.
    
    LATDbits.LATD1 = 0; // RS
    LATDbits.LATD0 = 0; // RW
    
    LATDbits.LATD6 = 0; // DB7
    LATDbits.LATD5 = 0; // DB6
    LATDbits.LATD4 = 1; // DB5
    LATBbits.LATB1 = 1; // DB4
    
    LATBbits.LATB0 = 0; // DB3
    LATEbits.LATE1 = 0; // DB2
    LATEbits.LATE2 = 0; // DB1
    LATDbits.LATD3 = 0; // DB0
    delayzz();
    tetikle();
}

void setInput(){ // Giris ayarlari. LCD'de gosterilen her karakterden
    //sonra imlec 1 saga kayar.
    
    LATDbits.LATD1 = 0; // RS
    LATDbits.LATD0 = 0; // RW
    
    LATDbits.LATD6 = 0; // DB7
    LATDbits.LATD5 = 0; // DB6
    LATDbits.LATD4 = 0; // DB5
    LATBbits.LATB1 = 0; // DB4
    
    LATBbits.LATB0 = 0; // DB3
    LATEbits.LATE1 = 1; // DB2
    LATEbits.LATE2 = 1; // DB1
    LATDbits.LATD3 = 0; // DB0
    delayzz();
    tetikle();
}

void setDDRAM_ADDR( unsigned char D6,unsigned char D5,unsigned char D4,unsigned char D3,unsigned char D2,unsigned char D1,unsigned char D0 ){
    // addres must be  6-bit address.
    LATDbits.LATD1 = 0; // RS
    LATDbits.LATD0 = 0; // RW
    
    LATDbits.LATD6 = 1; // DB7
    
    LATDbits.LATD5 = D6; // DB6
    LATDbits.LATD4 = D5; // DB5
    LATBbits.LATB1 = D4; // DB4
    
    LATBbits.LATB0 = D3; // DB3
    LATEbits.LATE1 = D2; // DB2
    LATEbits.LATE2 = D1; // DB1
    LATDbits.LATD3 = D0; // DB0
    delayzz();
    tetikle();
}

void setCGRAM_ADDR(unsigned char D5,unsigned char D4,unsigned char D3,unsigned char D2,unsigned char D1,unsigned char D0 ){
    LATDbits.LATD1 = 0; // RS
    LATDbits.LATD0 = 0; // RW
    
    LATDbits.LATD6 = 0; // DB7
    
    LATDbits.LATD5 = 1; // DB6
    LATDbits.LATD4 = D5; // DB5
    LATBbits.LATB1 = D4; // DB4
    
    LATBbits.LATB0 = D3; // DB3
    LATEbits.LATE1 = D2; // DB2
    LATEbits.LATE2 = D1; // DB1
    LATDbits.LATD3 = D0; // DB0
    delayzz();
    tetikle();
}

void writeToRAMBIN(unsigned char D7,unsigned char D6,unsigned char D5,unsigned char D4,unsigned char D3,unsigned char D2,unsigned char D1,unsigned char D0){ //girilen degeri o anki cursora yazar.
    
    LATDbits.LATD1 = 1; // RS
    LATDbits.LATD0 = 0; // RW
    
    LATDbits.LATD6 = D7; // DB7
    
    LATDbits.LATD5 = D6; // DB6
    LATDbits.LATD4 = D5; // DB5
    LATBbits.LATB1 = D4; // DB4
    
    LATBbits.LATB0 = D3; // DB3
    LATEbits.LATE1 = D2; // DB2
    LATEbits.LATE2 = D1; // DB1
    LATDbits.LATD3 = D0; // DB0
    delayzz();
    tetikle();
    
}

void writeToRAM(int a){ //Girilen int de?erinin kar??l???na gelen karakteri yazd?r?yor.
    accessiblebyte byte;
    byte.data=a;
    writeToRAMBIN(byte.DB7,byte.DB6,byte.DB5,byte.DB4,byte.DB3,byte.DB2,byte.DB1,byte.DB0);
    
}

void numbersToRAM(int a){
    if (a==9) writeToRAM(0x39);
    else if (a==8) writeToRAM(0x38);
    else if (a==7) writeToRAM(0x37);
    else if (a==6) writeToRAM(0x36);
    else if (a==5) writeToRAM(0x35);
    else if (a==4) writeToRAM(0x34);
    else if (a==3) writeToRAM(0x33);
    else if (a==2) writeToRAM(0x32);
    else if (a==1) writeToRAM(0x31);
    else if (a==0) writeToRAM(0x30);
    else writeToRAM(0x21);
    
}

void customChar(){
    /*LCD Custom Characters*/
    /*Left Side of TRIANGLE*/
    
    /*setCGRAM_ADDR(0,0,0,0,0,0);
    writeToRAMBIN(0,0,0,0,0,0,0,1);
    
    setCGRAM_ADDR(0,0,0,0,0,1);
    writeToRAMBIN(0,0,0,0,0,0,1,1);
    
    setCGRAM_ADDR(0,0,0,0,1,0);
    writeToRAMBIN(0,0,0,0,0,1,1,0);
    
    setCGRAM_ADDR(0,0,0,0,1,1);
    writeToRAMBIN(0,0,0,0,0,1,0,0);
    
    setCGRAM_ADDR(0,0,0,1,0,0);
    writeToRAMBIN(0,0,0,0,1,1,0,0);
    
    setCGRAM_ADDR(0,0,0,1,0,1);
    writeToRAMBIN(0,0,0,1,1,0,0,0);
    
    setCGRAM_ADDR(0,0,0,1,1,0);
    writeToRAMBIN(0,0,0,1,0,0,0,0);
    
    setCGRAM_ADDR(0,0,0,1,1,1);
    writeToRAMBIN(0,0,0,0,0,0,0,0);
    
    
    /*Right Side of TRIANGLE*/
    
   /*setCGRAM_ADDR(0,0,1,0,0,0);
    writeToRAM(0x10);
    
    setCGRAM_ADDR(0,0,1,0,0,1);
    writeToRAM(0x18);
    
    setCGRAM_ADDR(0,0,1,0,1,0);
    writeToRAM(0x0C);  
    
    setCGRAM_ADDR(0,0,1,0,1,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,0,1,1,0,0);
    writeToRAM(0x06);
    
    setCGRAM_ADDR(0,0,1,1,0,1);
    writeToRAM(0x03);
    
    setCGRAM_ADDR(0,0,1,1,1,0);
    writeToRAM(0x01);
    
    setCGRAM_ADDR(0,0,1,1,1,1);
    writeToRAM(0x00);
    
    /*Left Side of SQUARE WAVE*/
    /*setCGRAM_ADDR(0,1,0,0,0,0);
    writeToRAM(0x07);
    
    setCGRAM_ADDR(0,1,0,0,0,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,0,0,1,0);
    writeToRAM(0x04);  
    
    setCGRAM_ADDR(0,1,0,0,1,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,0,1,0,0);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,0,1,0,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,0,1,1,0);
    writeToRAM(0x1C);
    
    setCGRAM_ADDR(0,1,0,1,1,1);
    writeToRAM(0x00);
    
    /*Right Side of SQUARE WAVE*/
    /*setCGRAM_ADDR(0,1,1,0,0,0);
    writeToRAM(0x1C);
    
    setCGRAM_ADDR(0,1,1,0,0,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,1,0,1,0);
    writeToRAM(0x04);  
    
    setCGRAM_ADDR(0,1,1,0,1,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,1,1,0,0);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,1,1,0,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(0,1,1,1,1,0);
    writeToRAM(0x07);
    
    setCGRAM_ADDR(0,1,1,1,1,1);
    writeToRAM(0x00);
    
    /*Left Side of SINE WAVE*/
    /*setCGRAM_ADDR(1,0,0,0,0,0);
    writeToRAM(0x00);
    
    setCGRAM_ADDR(1,0,0,0,0,1);
    writeToRAM(0x00);
    
    setCGRAM_ADDR(1,0,0,0,1,0);
    writeToRAM(0x00);  
    
    setCGRAM_ADDR(1,0,0,0,1,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(1,0,0,1,0,0);
    writeToRAM(0x0A);
    
    setCGRAM_ADDR(1,0,0,1,0,1);
    writeToRAM(0x11);
    
    setCGRAM_ADDR(1,0,0,1,1,0);
    writeToRAM(0x11);
    
    setCGRAM_ADDR(1,0,0,1,1,1);
    writeToRAM(0x00);
    
      /*Right Side of SINE WAVE*/
    /*setCGRAM_ADDR(1,0,1,0,0,0);
    writeToRAM(0x11);
    setCGRAM_ADDR(1,0,1,0,0,1);
    writeToRAM(0x11);
    
    setCGRAM_ADDR(1,0,1,0,1,0);
    writeToRAM(0x0A);  
    
    setCGRAM_ADDR(1,0,1,0,1,1);
    writeToRAM(0x04);
    
    setCGRAM_ADDR(1,0,1,1,0,0);
    writeToRAM(0x00);
    
    setCGRAM_ADDR(1,0,1,1,0,1);
    writeToRAM(0x00);
    
    setCGRAM_ADDR(1,0,1,1,1,0);
    writeToRAM(0x00);
    
    setCGRAM_ADDR(1,0,1,1,1,1);
    writeToRAM(0x00);*/
    
}

void initUART()
{
    TRISCbits.RC6    = 1;       //direction of Tx and Rx pins
    TRISCbits.RC7    = 1;
       
    RXDTP = 0;      /*RX data received is not inverted.*/
    TXCKP = 0;      /*TX data is not inverted*/
    BRG16 = 0;      /*8-bit baud rate generator*/
    WUE = 0;        /*RX pin is not monitored or rising edge detected*/
    ABDEN = 0;      /*Baud rate measurement disabled.*/
    
    SPBRG = 25;      /* set the baud rate 9600 for 4 mhz clock */
     
    TX9  = 0;        /* 8- or 9-bit transmission */
    TXEN = 1;        /* enable the transmitter */
    SYNC = 0;        /* asynchronous */
    SENDB = 0;        /*send Sync Break on next transmission */
    BRGH = 1;         /*High Speed*/
     
    TXIE = 0;        /* disable tx interrupts */
    RCIE = 0;        /* disable rx interrupts */
    
    SPEN = 1;        /* enable serial port pins */
    RX9  = 0;        /* 8- or 9-bit reception */
    CREN = 1;        /* enable reception */
              
}


void transmit_char(char out)   { 
    while(TXIF==0); /*wait for transmit interrupt flag*/
    TXIF=0;
    TXREG=out; /*wait for transmit interrupt flag to set which indicates TXREG is ready
           for another transmission*/ 
}

char receive_char()
{
    while(RCIF==0);    // Wait till the data is received 
    RCIF=0;            // Clear receiver flag
    return(RCREG);     // Return the received data to calling function
}

void transmit_string(char str[],int sz){
    for(int i=0;i<sz-1;i++){
        transmit_char(str[i]);
    }
    
}

/*void initMenu(){
    char menu[]="<<<<<PicOscope Menu>>>>>";
    for(int i= 0;i<sizeof(menu);i++){
        transmit_char(menu[i]);    
    }
    
    transmit_char(0x0A);
    transmit_char(0x0D);
    transmit_char(0x0A);
    transmit_char(0x0D);
    
    char freqsel[]="To Detect Frequency Press --> F";
    
    for(int i= 0;i<sizeof(freqsel)-1;i++){
        transmit_char(freqsel[i]);    
    }
    
    transmit_char(0x0A);
    transmit_char(0x0D);
    
    
    char sigtype[]="To Detect Signal Type Press --> S";
    
    for(int i= 0;i<sizeof(sigtype)-1;i++){
        transmit_char(sigtype[i]);    
    }
    
    transmit_char(0x0A);
    transmit_char(0x0D);
    
    
    char volt[]="To Detect Voltage Press --> V";
    
    for(int i= 0;i<sizeof(volt)-1;i++){
        transmit_char(volt[i]);    
    }
    
    transmit_char(0x0A);
    transmit_char(0x0D);
    
    char anlyze[]="To Analyze the Signal widely Press --> A \n ~";
    
    for(int i= 0;i<sizeof(anlyze)-1;i++){
        transmit_char(anlyze[i]);    
    }
    
    transmit_char(0x0A);
    transmit_char(0x0D);
    
}*/



void main(void) {
    
    

    
    
    
    
    
    accessiblebyte RAMbyte;
    int result=0;
                 // OUTPUT PINS FOR LCD
    TRISD = 0x00; // All PORT D pins assigned as output.
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE1 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
   
    
        //Data assigned for derivative operations.
    unsigned int datas[50];
    int derivdatas[49];
    int deriv2datas[48];

    
    TRISAbits.TRISA0=1; // Analog giris input olarak ayarlandi.
     
    TRISBbits.TRISB2=0;    //Debug LED
    TRISDbits.TRISD7=0;
        // Variables assigned to calculate frequency.
    unsigned int signal_period,data1,data2;
    unsigned int Frequency;
    
    TRISCbits.TRISC2=1; //CCP 1 girisi. Pin 17
     
    
        
                   //ADC configs
    ADCON0=0b00000000; //Channel 0 (AN0)selected. ADC is disabled.
    ADCON1=0b00001110; // AN0 set as analog and VREFs set as Vdd and Vss.
    ADCON2=0b10001100; //Right justified. 4TOSC(FOSC/4). 2Tad.
    
    ADCON0bits.ADON = 1; // ADC enabled.
    
    dispONOFF();
    
    displaySil();
    
    setFunc();
    
    setInput();
    
    writeToRAMBIN(0,1,1,0,0,1,1,0); // f
 
    
    RAMbyte.data = 0x3A;
    writeToRAMBIN(RAMbyte.DB7,RAMbyte.DB6,RAMbyte.DB5,RAMbyte.DB4,RAMbyte.DB3,RAMbyte.DB2,RAMbyte.DB1,RAMbyte.DB0);

    initUART();
    //initMenu();
    
    T3CCP2=0;
    T3CCP1=0;
    INTCONbits.GIE = 1;
    PIE1bits.CCP1IE=1;
    PIR1bits.CCP1IF=0;     
    CCP1CON=0x05;       /* Capture mode is selected for detecting Rising edge */
    CCPR1=0x00;         /*CCPR1 is capture count Register which is cleared initially*/
    TMR1IF=0;           
    T1CON=0x80;         /* Enable 16-bit TMR1 Register,No pre-scale,use internal clock,Timer OFF */
    TMR1=0; 
    
  
          
    
    
    
    //char sel = receive_char();
    
   
    
    while (1)   {
        
        
        
        
            
        TMR1ON=1;           /* Turn-On Timer1 */
        
        while(!PIR1bits.CCP1IF);  /*Wait for Interrupt flag which is generated when edge is detected*/
        PIR1bits.CCP1IF=0;
        data1 = CCPR1;              /*Copy count of 1st edge detected*/  
        
        while(!PIR1bits.CCP1IF);  /*Wait for Interrupt flag which is generated when edge is detected*/
        PIR1bits.CCP1IF=0;        
        data2 = CCPR1;              /*Copy count of 2nd edge detected*/
            
        _delay(100);
        setDDRAM_ADDR(0,0,0,0,0,1,0);
            
        unsigned int temp1 = data1;
            
        unsigned int temp2 = data2;
            
        unsigned int temp3 = temp2-temp1;
               
        Frequency = 1000000 /temp3;
            
        
        
        unsigned int temp = Frequency;
        
            unsigned int onbins = (temp%100000)/10000;
            unsigned int bins=(temp%10000)/1000;
            unsigned int yuzs=(temp%1000)/100;
            unsigned int ons=(temp%100)/10;
            unsigned int ones=(temp%10);
            
            
            numbersToRAM(onbins);
            numbersToRAM(bins);
            numbersToRAM(yuzs);
            numbersToRAM(ons);
            numbersToRAM(ones);
        
            //transmit_char(102);
            //transmit_char(58);
            transmit_char(48+onbins);
            transmit_char(48+bins);
            transmit_char(48+yuzs);
            transmit_char(48+ons);
            transmit_char(48+ones);
        
            
            
            
            
        unsigned int j=0;
        while(j<50) {
                              
            ADCON0bits.GO=1; // Bu bit 1 oldugu surece cevirim bitmedi demek.        
            while(ADCON0bits.GO == 1); // wait for conversion.        
            result = (ADRESH<<8)+ADRESL;
            datas[j]=(unsigned long)result;
            sample_timer(temp3);
            j++;
        }
        
        
        
        
            j=0;
        
            while(j<50) {
            
                int temp4 = datas[j];
                unsigned int onbins1 = (temp4%100000)/10000;
                unsigned int bins1=(temp4%10000)/1000;
                unsigned int yuzs1=(temp4%1000)/100;
                unsigned int ons1=(temp4%100)/10;
                unsigned int ones1=(temp4%10);        
            
                transmit_char(48+onbins1);
                transmit_char(48+bins1);
                transmit_char(48+yuzs1);
                transmit_char(48+ons1);
                transmit_char(48+ones1);
                             
                j++;
            }
        
        j=1;
        
                
        
        while(j<50) {
            if((datas[j]<datas[j-1])){
                derivdatas[j-1]=(datas[j]-datas[j-1])*(-1);
            }
            else    {
                derivdatas[j-1]=(datas[j]-datas[j-1]);
            }
            
            j++;
            
        }
        
        int predic = 40;
        
        j=0;
        while(j<49) {
            if(derivdatas[j]<= predic/2){
                derivdatas[j]=0;
            }
            else if(derivdatas[j]>= predic/2)   {
                derivdatas[j]=1;
            }
            
            j++;
            
        }
        
        
        
            j=0;
        
            while(j<49) {
            
                unsigned int temp5 = derivdatas[j];
                unsigned int onbins2 = (temp5%100000)/10000;
                unsigned int bins2=(temp5%10000)/1000;
                unsigned int yuzs2=(temp5%1000)/100;
                unsigned int ons2=(temp5%100)/10;
                unsigned int ones2=(temp5%10);        
            
                transmit_char(48+onbins2);
                transmit_char(48+bins2);
                transmit_char(48+yuzs2);
                transmit_char(48+ons2);
                transmit_char(48+ones2);
          
            
            j++;
            
            }
        
        j=1;
        
        
        
        while(j<48) {
            deriv2datas[j-1]=(abs(derivdatas[j]-derivdatas[j-1]));
            if(deriv2datas[j-1]<=20)    {
                deriv2datas[j-1]=0;
            }
            j++;
            
        }
        
        
        
        
            j=0;
        
            while(j<48) {
           
            
                unsigned int temp6 = deriv2datas[j];
                unsigned int onbins3 = (temp6%100000)/10000;
                unsigned int bins3=(temp6%10000)/1000;
                unsigned int yuzs3=(temp6%1000)/100;
                unsigned int ons3=(temp6%100)/10;
                unsigned int ones3=(temp6%10);        
            
            
                transmit_char(48+onbins3);
                transmit_char(48+bins3);
                transmit_char(48+yuzs3);
                transmit_char(48+ons3);
                transmit_char(48+ones3);
            
       
            
                j++;
            }
        
        
        
        
        
        int countd1d = 0, curr_cnt = 1, freq_d1d = 7, key = 0,k=0,m=0;
        for(k = 0; k < 48; k++)    {
            key = derivdatas[k];
            for(m =k+1; m < 49; m++) {
                if(key == derivdatas[m] && freq_d1d != key)  {
                    curr_cnt++;
                }
            }
            if(countd1d < curr_cnt)    {
                countd1d = curr_cnt; // frequency
                curr_cnt = 1;
                freq_d1d = key; //Most Frequent index
            }       
        }
        
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(48+freq_d1d);
        
        unsigned int cnt11=(countd1d%100)/10;
        unsigned int cnt12=countd1d%10;
        
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(48+cnt11);
        transmit_char(48+cnt12);
        
        
             
        int countd2d = 0, curr_cnt2 = 1, freq_d2d = 7, key2 = 0,k2=0,m2=0;
        for(k2 = 0; k2 < 47; k2++)    {
            key2 = deriv2datas[k2];
            for(m2 =k2+1; m2 < 48; m2++) {
                if(key2 == deriv2datas[m2] && freq_d2d != key2)  {
                    curr_cnt2++;
                }
            }
            if(countd2d < curr_cnt2)    {
                countd2d = curr_cnt2; // frequency
                curr_cnt2 = 1;
                freq_d2d = key2; //Most Frequent index
            }       
        }
        
        
        
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(48+freq_d2d);
        
        unsigned int cnt21=(countd1d%100)/10;
        unsigned int cnt22=countd1d%10;
        
        
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(0x30);
        transmit_char(48+cnt21);
        transmit_char(48+cnt22);
       
        
        
        if(freq_d1d == 0 && freq_d2d==0){
            setDDRAM_ADDR(0,0,1,1,0,1,0); //imlec sagdan 3. indekse gitti.
            writeToRAM(0x53);   //S
            writeToRAM(0x51);   //Q
            writeToRAM(0x55);   //U
            char sq[]="This is a square wave\n~";
            int sz6=sizeof(sq);
            transmit_string(sq,sz6);
            
            
            
        }// square wave
        
        if(freq_d1d != 0 && freq_d2d == 0){
            setDDRAM_ADDR(0,0,1,1,0,1,0); //imlec sagdan 3. indekse gitti.
            writeToRAM(0x54);   //T
            writeToRAM(0x52);   //R
            writeToRAM(0x69);   //i
            
            char tr[]="This is a triangle wave~";
            int sz7=sizeof(tr);
            transmit_string(tr,sz7);
            
            
        }// triangle wave
        
        if(freq_d2d != 1 && freq_d1d != 0 && freq_d2d != 0 && freq_d1d != 1){
            setDDRAM_ADDR(0,0,1,1,0,1,0); //imlec sagdan 3. indekse gitti.
            writeToRAM(0x53);   //S
            writeToRAM(0x69);   //i
            writeToRAM(0x4E);   //N
            
            char sn[]="This is a sine wave \0~";
            int sz8=sizeof(sn);
            transmit_string(sn,sz8);
            
            
        }// triangle wave
        
       
    
    }
}
