//===========================================================================================================
//
//	Author				:Cytron Technologies		
//	Project				:Interface Free Controller-Play Station Controller card (IFC-PS01)
//	Project description		:Sample for IFC-PS01
//  	Date				: 15 September 2008
//
//===========================================================================================================

//	include card h file
//===========================================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_cp.h"
#include "iic_ps.h"

//============================================================================================================
//	configuration bit, user is advised not to change the setting here
//============================================================================================================
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = ON 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//============================================================================================================
//	define input or output of board here
//	For example:
///				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//============================================================================================================
#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00
#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00
#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards


//============================================================================================================
//	card address card here
//	User can define the address of slave
//============================================================================================================

#define ps_add		0b010101		//Please make sure the communication address on
									//IFC-PS01 is compatible with the address.


//============================================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//============================================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);

void uart_send(unsigned char data);
void uart_dec(unsigned char data);
void uart_hex(unsigned char data);
void uart_str(static const rom char *s);

//===========================================================================================================
//	global variable
//===========================================================================================================
unsigned long i;

//===========================================================================================================	
//	main function
//	This is the main function where program start to execute
//===========================================================================================================
void main(void)
{
	unsigned char temp1, temp2, temp3;
	
	//init
	init();
	
		
	cp1_goto(0,0);
	cp1_str("set PS01");
	cp1_goto(1,0);
	cp1_str("add to 010101");
	
	while(cp1_sw(1));
	while(!cp1_sw(1));
	delay(10000);	
	cp1_clr();

	while(1)
	{	
		//test switch
		while(cp1_sw(1))
		{
			cp1_goto(0,0);
			cp1_str("1. all sw");
			
			cp1_goto(1,0);
			if(!ps_sw(ps_add,p_circle))cp1_str("circle");
			else if(!ps_sw(ps_add,p_cross))cp1_str("cross");
			else if(!ps_sw(ps_add,p_triangle))cp1_str("triangle");
			else if(!ps_sw(ps_add,p_square))cp1_str("square");
			else if(!ps_sw(ps_add,p_up))cp1_str("up");
			else if(!ps_sw(ps_add,p_down))cp1_str("down");
			else if(!ps_sw(ps_add,p_left))cp1_str("left");
			else if(!ps_sw(ps_add,p_right))cp1_str("right");
			else if(!ps_sw(ps_add,p_select))cp1_str("select");
			else if(!ps_sw(ps_add,p_start))cp1_str("start");
			else if(!ps_sw(ps_add,p_joyl))cp1_str("joyL");
			else if(!ps_sw(ps_add,p_joyr))cp1_str("joyR");
			else if(!ps_sw(ps_add,p_l1))cp1_str("L1");
			else if(!ps_sw(ps_add,p_l2))cp1_str("L2");
			else if(!ps_sw(ps_add,p_r1))cp1_str("R1");
			else if(!ps_sw(ps_add,p_r2))cp1_str("R2");
			else if(!ps_stat(ps_add))cp1_str("no con");
			else cp1_str("        ");
		}
		
		while(!cp1_sw(1));
		delay(10000);
		cp1_clr();
		
		//test analog information
		temp1=1;		//use to indicate current submode (default=1)
		while(cp1_sw(1))
		{	
			cp1_goto(0,0);				//display current mode
			cp1_str("2. joystick");
			
			cp1_goto(0,12);				//display current submode
			cp1_char('A'+temp1-1);
			
			if(!cp1_sw(2))				//button to change submode
			{
				while(!cp1_sw(2));
				if(temp1<3)temp1+=1;	
				else temp1=1;
			}
			
			switch(temp1)				//perform submode task
			{
				case 1:
					cp1_goto(1,0);
					cp1_dec(ps_joy(ps_add,joy_lx),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_ly),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_rx),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_ry),3);
					cp1_char(' ');
					break;
				case 2:
					cp1_goto(1,0);
					cp1_dec(ps_joy(ps_add,joy_lu),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_ld),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_ll),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_lr),3);
					cp1_char(' ');
					break;
				case 3:
					cp1_goto(1,0);
					cp1_dec(ps_joy(ps_add,joy_ru),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_rd),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_rl),3);
					cp1_char(' ');
					cp1_dec(ps_joy(ps_add,joy_rr),3);
					cp1_char(' ');
					break;
			}
		}
		
		while(!cp1_sw(1));
		delay(10000);
		cp1_clr();
		
		//test vibrator
		while(cp1_sw(1))
		{	
			cp1_goto(0,0);				//display current mode
			cp1_str("3. vibrator");
	
			cp1_goto(1,0);
			if(!cp1_sw(2))
			{
				ps_vibrate(ps_add,1,1);
				cp1_str("small motor on");
			}
			else if(!cp1_sw(3))
			{
				ps_vibrate(ps_add,2,150);
				cp1_str("big motor 150");
			}
			else if(!cp1_sw(4))
			{
				ps_vibrate(ps_add,2,255);
				cp1_str("big motor 255");
			}		
			else
			{
				ps_vibrate(ps_add,1,0);
				ps_vibrate(ps_add,2,0);
				cp1_str("                ");
			}
		}
	
		while(!cp1_sw(1));
		delay(10000);
		cp1_clr();
	}
}


//===========================================================================================================
//	functions
//	User can write all the necessary function here
//===========================================================================================================
void delay(unsigned long data)
{
	// this is a delay function for user to use when the program needs a delay
	// this function can be call by typing : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}

void init(void)
{
	// this is a initialization to set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;				// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;				// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'

	
	//interrupt
	RCONbits.IPEN = 0;		// Enable priority interrupt
	INTCONbits.GIEH = 0;
	INTCONbits.GIEL = 0;

	//initialize uart	
	TRISCbits.TRISC6=0;		//tx pin
	TRISCbits.TRISC7=1;		//rx pin
	SPBRG = 21;				//115200
	TXSTAbits.TXEN = 1;
	TXSTAbits.BRGH = 1;
	RCSTAbits.SPEN = 1;
	RCSTAbits.CREN = 1;

}

//==============================================================================================================
//	uart function
//==============================================================================================================
void uart_send(unsigned char data)
{
	while (PIR1bits.TXIF == 0);
	TXREG = data;
}

void uart_dec(unsigned char data)
{
	uart_send(0x30+data/100);
	data=data%100;
	uart_send(0x30+data/10);
	data=data%10;
	uart_send(0x30+data);	
}

void uart_hex(unsigned char data)
{
	unsigned char temp1;
	uart_send(' ');
	temp1=data/16;
	if(temp1>9)uart_send('a'+(temp1-10));
	else uart_send(0x30+temp1);
	data=data%16;
	if(data>9)uart_send('a'+(data-10));
	else uart_send(0x30+data);
}

void uart_str(static const rom char *s)
{
	while(*s)uart_send(*s++);
}

//===============================================================================================================
//	Interrupt vector
//================================================================================================================
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		//jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow		//jump to interrupt routine
	_endasm
}
#pragma code

//================================================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//================================================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

