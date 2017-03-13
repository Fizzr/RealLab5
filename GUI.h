/*
 * GUI.h
 *
 * Created: 2015-02-27 11:28:23
 *  Author: alerne-2
 */ 


#ifndef GUI_H_
#define GUI_H_
#include "TinyTimber.h"
#include <avr/io.h>

typedef struct{
	Object super;
}GUI;


#define initGUI() {initObject()}
	
void printAt(long num, int pos);

void printAt1(long num);
void printAt2(long num);

void writeChar(char ch, int pos);
void decodeChar(char ch, int* decoded);

#endif /* GUI_H_ */