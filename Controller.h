
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <avr/io.h>
#include "TinyTimber.h"
#include "GUI.h"

typedef enum
{
	north,
	south,
	none
}Direction;

//Inputs
#define NORTHARRIVE 0
#define NORTHENTRY 1
#define SOUTHARRIVE 2
#define SOUTHENTRY 3

//Input - Status on lights
#define NORTHGREENLIGHT 0
#define NORTHREDLIGHT 1
#define SOUTHGREENLIGHT 2
#define SOUTHREDLIGHT 3

// Delay
#define maxTime MSEC(20000)		//20 sec max allowed uptime for cars. 20 cars can cross. Total 30 sec wait time on other side (5 sec "open", 20 sec pass, 5 sec "close");
#define bridgeTime MSEC(5000) // 5 seconds to cross bridge
#define updateDelay MSEC(1000) // Updating state every 1 second
#define maxBridgeCount 5 // maximum number of cars to cross, 

#define maxCross 20

#define aRed 10 // 1010
#define NredSgreen 6 // 0110
#define NgreenSred 9 // 1001

#define PAL 0

typedef struct{
	Object super;
	int SCOB; // South Car on Bridge
	int NCOB; // North Car on Bridge
	int southQueue;
	int northQueue;
	int carsLetThru;
	Direction lastLight;
	int currentStatus;
// 	int generation;
// 	int timeLimit;
}Controller;

void receiveData(Controller *self, int input);
void sendData(Controller *self, int output);
void carExit(Controller *self, int a);
void start(Controller *self,int a);
void handleBridge(Controller *self);


#define initController(G){initObject(),0,0,0,0,0, none, aRed/*, 0, 0*/};

#endif /* CONTROLLER_H_ */