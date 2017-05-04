#define F_CPU 8000000// Clock Speed

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TinyTimber.h"
#include "USART.h"
#include "GUI.h"
#include <util/delay.h>


//#define MYUBRR ((FOSC/16/BAUD)-1)
//#define FOSC 8000000// Clock Speed
//#define BAUD 9600

//#define BAUDRATE_2400 207

#define UBRR 51//((FOSC/(16*BAUD))-1) 


int main(void){

	initLCD();
	USART_Init();
	
	Controller controller = initController();
	USART usart = initUSART(&controller);
	INSTALL(&usart,receiveH,IRQ_USART0_RX);
	sei();
	return TINYTIMBER(&controller,start,0);

}

void USART_Init(){
	
	
	/*Set baud rate */
	//unsigned char MYUBRR = 51;
	UBRR0H = UBRR >> 8 ;
	UBRR0L = UBRR;
	/* Enable interrupts |	receiver	| transmitter */
	UCSR0B = (1<<RXCIE0) |	(1<<RXEN0)	| (1<<TXEN0);
	/*Synchronous mode (U_S_ART). frame format: 8-bit data. no parity, 1 stop bit (no usbs = 1 stop) */
	UCSR0C = /*(1<< UMSEL0) |*/(1<<UCSZ01)|(1<<UCSZ00); /*(1<<USBS0)|*/

}


void initLCD(void){
	
			CLKPR  = 0x80;		//
			CLKPR  = 0x00;		//Disables CPU clock prescaler
			// Driver time to 300
			// contrast control V to 3.35V
			LCDCCR= (0<<LCDDC2) | (0<<LCDDC1) | (0<<LCDDC0) | /* Voltage */ (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0);
			//------------------
			// external asynchronous clock source
			// 1/3 bias
			// 1/4 duty cycle
			// 25 segments
			LCDCRB = (1<<LCDCS) | (1<<LCDMUX1) | (1<<LCDMUX0) | (0<<LCD2B) | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);
			//------------------
			// Prescaler N=16
			// clock divider D=8
			LCDFRR = (0<<LCDPS2) | (0<<LCDPS1) | (0<<LCDPS0) | (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
			//--------------
			// LCD enable
			// low power waveform
			// no frame interrupt
			// no blanking
			LCDCRA  = (1<<LCDEN) | (0<<LCDAB) | (0<<LCDIE) | (0<<LCDBL);
			//-------------
			TCCR1B = (1<<CS12) | (0<<CS11) | (0<<CS10); // 0x0D		//Clock prescaler 1024
			EIFR = 0xC0;
			EIMSK = 0xC0;

}
