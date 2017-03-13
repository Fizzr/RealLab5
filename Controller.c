#include "Controller.h"
#include <util/delay.h>


void start(Controller *self, int a)
{
	printAt(9,5);
	int counter = 0;
	while(1)
	{
		while(!(UCSR0A) & (1<<RXC0));
			
		//while (!( UCSR0A & (1<<UDRE0)));                // wait while register is free
		//UDR0 = 't';
		//_delay_ms(1000);
		//
		counter++;
		//printAt(counter,0);
		unsigned int data = UDR0;
		int i;
		for(i = 0; i > 4; i++)
			printAt((data/(1000/(10*i)))%(10*i), i);
		//ASYNC(self, LightStateChange,0);
	}
}




// Receive Data
void receiveData(Controller *self, int input){

	printAt(4,5);
	int southArrival = input & (1 << SOUTHARRIVE);
	int southEntry = input & (1 << SOUTHENTRY);
	int northArrival = input & (1 << NORTHARRIVE);
	int northEntry = input & (1 << NORTHENTRY);

	// Arrival of Cars from South and North
	if(southArrival){
		self->southQueue++;
	}
	if(northArrival){
		self->northQueue++;
	}

	// Cars entrying the bridge from South and North
	if(southEntry){
		self->southQueue--;
		self->SCOB++; // South Car on Bridge (Minus when they leave)
		AFTER(bridgeTime, self, southCarExit, PAL);
	}
	if(northEntry){
		self->northQueue--;
		self->NCOB++; // North Car on Bridge (Minus when they leave)
		AFTER(bridgeTime, self, northCarExit, PAL);
	}

}


// Send Data
void sendData(Controller *self, int output){
	//Buffer must be empty 
// 	while (!(UCSR0A &(1<<UDRE0))){
// 
// 	}
	UDR0 = output;
}

// LightStateChange
void LightStateChange(Controller *self,int a){

	// BOTH QUEUE = EMPTY
	if(self->southQueue==0 && self->northQueue==0){
		self->lightState == aRed;
		self->lightStatep == self->lightState;
	}
	//SouthCarsQueue Manager
	else if(self->southQueue > 0 && self->northQueue == 0){
		if(self->NCOB == 0){
			self->lightState == NredSgreen;
			self->lightStatep == self->lightState;
		}else{
			self->lightStatep == NgreenSred;
		}
	}

	//NorthCarsQueue Manager
	else if(self->northQueue > 0 && self->southQueue ==0){
		if(self->SCOB == 0){
			self->lightState == NgreenSred;
			self->lightStatep == self->lightState;
		}else{
			self->lightStatep == NredSgreen;
		}
	}

	//BothQueue Manager 


	//SouthCars on Bridge
	else if(self->SCOB){
		self->scBridgeCount++; // Counts cars that drives over bridge
		if(self->scBridgeCount > maxBridgeCount){ // Checking how many cars that have been let through
			self->scBridgeCount = 0; // Empty the bridge and switch lights for next update. 
			self->lightState = NredSgreen;
		}else{
			//let sc go through
			self->lightState = NredSgreen;
			self->lightStatep = self->lightState;
		}

	}
	//NorthCars on Bridge
	else if(self->NCOB){
		self->ncBridgeCount++; // Counts cars that drives over bridge
		if(self->ncBridgeCount > maxBridgeCount){ // Checking how many cars that have been let through
			self->ncBridgeCount = 0; // Empty the bridge and switch lights for next update. 
			self->lightState = NgreenSred;
		}else{
			// North Cars through
			self->lightState = NgreenSred;
			self->lightStatep = self->lightState;
		}
	}
	//ASYNC(self,sendData,self->lightState); //Send the data to the avr
	//ASYNC(self,sendData,aRed); // set all to red
	//printAt(3,5);
	//AFTER(updateDelay,self, sendData, 4);
	AFTER(updateDelay,self,LightStateChange,0);

}

// Cars Leaving the bridge on south side (NorthCarExit)
void northCarExit(Controller *self,int a){
	self->NCOB--;
}

// Cars leaving the bridge on north side (SouthCarExit)
void southCarExit(Controller *self, int a){
	self->SCOB--;
}