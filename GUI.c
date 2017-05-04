

#include "GUI.h"



// Needed for printing chars
void decodeChar(char ch, int* decoded)
{
	switch(ch)
	{
		case '0':
		decoded[0] = 0x11;
		decoded[1] = 0x55;
		decoded[2] = 0x55;
		decoded[3] = 0x11;
		break;
		case '1':
		decoded[0] = 0;
		decoded[1] = 0x11;
		decoded[2] = 0x11;
		decoded[3] = 0;
		break;
		case '2':
		decoded[0] = 0x11;
		decoded[1] = 0x11;
		decoded[2] = 0xEE;
		decoded[3] = 0x11;
		break;
		case '3':
		decoded[0] = 0x11;
		decoded[1] = 0x11;
		decoded[2] = 0xBB;
		decoded[3] = 0x11;
		break;
		case '4':
		decoded[0] = 0;
		decoded[1] = 0x55;
		decoded[2] = 0xBB;
		decoded[3] = 0;
		break;
		case '5':
		decoded[0] = 0x11;
		decoded[1] = 0x44;
		decoded[2] = 0xBB;
		decoded[3] = 0x11;
		break;
		case '6':
		decoded[0] = 0x11;
		decoded[1] = 0x44;
		decoded[2] = 0xFF;
		decoded[3] = 0x11;
		break;
		case '7':
		decoded[0] = 0x11;
		decoded[1] = 0x11;
		decoded[2] = 0x11;
		decoded[3] = 0;
		break;
		case '8':
		decoded[0] = 0x11;
		decoded[1] = 0x55;
		decoded[2] = 0xFF;
		decoded[3] = 0x11;
		break;
		case '9':
		decoded[0] = 0x11;
		decoded[1] = 0x55;
		decoded[2] = 0xBB;
		decoded[3] = 0;
		break;
		case '#':
		decoded[0] = 0x22;
		decoded[1] = 0;
		decoded[2] = 0;
		decoded[3] = 0;
		break;
		default:
		decoded[0] = 0;
		decoded[1] = 0;
		decoded[2] = 0;
		decoded[3] = 0;
		break;
	}
}
//Needed for printing chars
void writeChar(char ch, int pos)
{
	if(0 > pos || pos > 5)
	{
		return;
	}
	
	int bitMask = (pos % 2 == 0) ? 0xF0 : 0x0F;
	int decoded[4];
	int *temp;
	temp = decoded;
	decodeChar(ch, temp);
	switch(pos/2)
	{
		case 0:
		LCDDR0 = LCDDR0 & bitMask;
		LCDDR5 = LCDDR5 & bitMask;
		LCDDR10 = LCDDR10 & bitMask;
		LCDDR15 = LCDDR15 & bitMask;
		bitMask = (~bitMask) & (0xFF);
		LCDDR0 = LCDDR0 | (bitMask & decoded[0]);
		LCDDR5 = LCDDR5 | (bitMask & decoded[1]);
		LCDDR10 = LCDDR10 | (bitMask & decoded[2]);
		LCDDR15 = LCDDR15 | (bitMask & decoded[3]);
		
		break;
		case 1:
		LCDDR1 = LCDDR1 & bitMask;
		LCDDR6 = LCDDR6 & bitMask;
		LCDDR11 = LCDDR11 & bitMask;
		LCDDR16 = LCDDR16 & bitMask;
		bitMask = (~bitMask) & (0xFF);
		LCDDR1 = LCDDR1 | (bitMask & decoded[0]);
		LCDDR6 = LCDDR6 | (bitMask & decoded[1]);
		LCDDR11 = LCDDR11 | (bitMask & decoded[2]);
		LCDDR16 = LCDDR16 | (bitMask & decoded[3]);
		break;
		case 2:
		LCDDR2 = LCDDR2 & bitMask;
		LCDDR7 = LCDDR7 & bitMask;
		LCDDR12 = LCDDR12 & bitMask;
		LCDDR17 = LCDDR17 & bitMask;
		bitMask = (~bitMask) & (0xFF);
		LCDDR2 = LCDDR2 | (bitMask & decoded[0]);
		LCDDR7 = LCDDR7 | (bitMask & decoded[1]);
		LCDDR12 = LCDDR12 | (bitMask & decoded[2]);
		LCDDR17 = LCDDR17 | (bitMask & decoded[3]);
		break;
	}

}
void printAt(long num, int pos) 
{
	writeChar( (num % 100) / 10 + '0', pos);
	pos++;
	writeChar( num % 10 + '0', pos);
}
