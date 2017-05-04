#include "USART.h"



void receiveH(USART *self, int a){
	// wait for recevie and set data when in buffer
	
	while (!(UCSR0A & (1<<RXC0)));  //Wait til receive complete bit set.
	unsigned int data = UDR0;
	ASYNC(self->ctrl, receiveData, data);
}