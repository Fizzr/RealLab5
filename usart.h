


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include "TinyTimber.h"
#include "Controller.h"

typedef struct{
	Object super;
	Controller *ctrl;
}USART;

#define initUSART(CTRL){initObject(), CTRL}

void receiveH(USART *self, int a);

#endif /* USART_H_ */