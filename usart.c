#include "USART.h"



void receiveH(USART *self, int a){
	// wait for recevie and set data when in buffer
	
	unsigned int data = UDR0;
	ASYNC(self->ctrl, receiveData, data);
	printAt(13,5);
}