#include "Controller.h"
#include <util/delay.h>


void start(Controller *self, int a)
{
	//ASYNC(self, LightStateChange,0);
}




// Receive Data
void receiveData(Controller *self, int input){

	int southArrival = input & (1 << SOUTHARRIVE);
	int southEntry = input & (1 << SOUTHENTRY);
	int northArrival = input & (1 << NORTHARRIVE);
	int northEntry = input & (1 << NORTHENTRY);

	// Arrival of Cars from South and North
	if(southArrival)
		self->southQueue++;
	if(northArrival)
		self->northQueue++;


	// Cars entering the bridge from South and North
	if(southEntry){
		self->southQueue--;
		self->carsLetThru++;
		self->SCOB++; // South Car on Bridge (Minus when they leave)
		AFTER(bridgeTime, self, carExit, (int) south);
	}
	if(northEntry){
		self->northQueue--;
		self->carsLetThru++;
		self->NCOB++; // North Car on Bridge (Minus when they leave)
		AFTER(bridgeTime, self, carExit, (int) north);
	}

	handleBridge(self);
}


// Send Data
void sendData(Controller *self, int output)
{
	self->currentStatus = output;
	//Buffer must be empty 
 	while (!(UCSR0A &(1<<UDRE0)));  

	UDR0 = output;
}

void carExit(Controller *self, int a)
{
	Direction dir = (Direction) a;
	switch(dir)
	{
		case(south):
			self->SCOB--;
			break;
		case(north):
			self->NCOB--;
			break;
	}
	handleBridge(self);
}

// void timeCheck(Controller *self, int startGeneration)
// {
// 	self->timeLimit = 1;
// }

void setLightsTo(Controller *self, Direction dir)
{
// 	self->timeLimit = 0;
// 	self->generation++;
// 	int a = self->generation; 
// 	AFTER(maxTime, self, timeCheck, a);

	int output;
	switch(dir)
	{
		case(south):
			output = NredSgreen;
			break;
		case(north):
			output = NgreenSred;
			break;
		case(none):
			output = aRed;
			break;
	}
	self->carsLetThru = 0;
	self->lastLight = dir;
	sendData(self, output);
}



/*
Called every time something significant happens in the status of the bridge,
like a car arriving, leaving or such. Therefore can be treated as a sort of
event stepper. Given a certain state we can assume a future, predictable state to execute shortly.
*/
void handleBridge(Controller *self)
{
	//if empty, administrative tasks!
	if(self->southQueue + self->northQueue + self->SCOB + self->NCOB == 0)
	{
		setLightsTo(self, none);
	}
	
	//South has cars, north does not
	else if(self->southQueue > 0 && self->northQueue == 0)	
	{
		if(self->NCOB == 0)	//no cars on bridge
		{
			if(self->lastLight != south)
				setLightsTo(self, south);
				
			//don't need to send anything, if south light is still green
		}
		else
			sendData(self, aRed);	//Close and 'claim' lights, then just wait for ping from when car leaves
	}
	//North has cars, north does not
	else if(self->northQueue > 0 && self->southQueue == 0)
	{
		if(self->SCOB == 0)	//no cars on bridge
		{
			if(self->lastLight != north)
				setLightsTo(self, north);
				
			//don't need to send anything, if north light is still green
		}
		else
			sendData(self, aRed);	//Close and 'claim' lights, then just wait for ping from when car leaves
	}
	else    //There are cars in both queues
	{
		// if the lights are red, there are cars on both sides of the bridge 
		// it means we're in the process of changing lights. If the bridge 
		// is not empty, then just let it pass!
		if(self->currentStatus == aRed && self->NCOB + self->SCOB == 0)	
		{
			if(self->lastLight == north || self->lastLight == none)		//if none, just go with south, fuck it!
				setLightsTo(self, south);
			else if(self->lastLight == south)
				setLightsTo(self, north);
		}
// 		else if(self->timeLimit == 1)
// 			sendData(self, aRed);
		else if(self->currentStatus == NgreenSred)
		{
			if(self->carsLetThru > maxCross)
				sendData(self, aRed); //close lights to switch later!
		}
		else if(self->currentStatus == NredSgreen)
		{
			if(self->carsLetThru > maxCross)
				sendData(self, aRed); //close lights to switch later!
		}
	}
	
	printAt(self->northQueue, 0);
	printAt(self->southQueue, 4);
	printAt(self->carsLetThru, 2);	
}




































