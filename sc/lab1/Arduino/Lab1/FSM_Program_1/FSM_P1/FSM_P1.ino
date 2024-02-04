#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

unsigned long initialTimeLED1 = 0; 
unsigned long initialTimeLED2 = 0; 
long intervalLED1 = 1000; // 1 second for SM1 
long intervalLED2 = 2500; // 2.5 seconds for SM2 

int stateLED1 = HIGH; 
int stateLED2 = HIGH;


// initialization 
void setup() 
{ 
  
  Timer1.initialize();
  MFS.initialize(&Timer1);
  
  initialTimeLED1 = millis(); 
  initialTimeLED2 = millis(); 

}

// run 
void loop() { 
//State machine 1 
  if ((millis() - initialTimeLED1) < intervalLED1) 
  { 
    MFS.writeLeds(LED_1, stateLED1);  
  } 
  else { 
    stateLED1 = !stateLED1; 
    initialTimeLED1 = millis(); 
  } 
  
//State machine 2 
  if ((millis() - initialTimeLED2) < intervalLED2) 
  { 
     MFS.writeLeds(LED_2, stateLED2); 
  } 
  else 
  { 
    stateLED2 = !stateLED2; 
    initialTimeLED2 = millis(); 
  }

}