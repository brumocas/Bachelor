#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

unsigned long initialTimeLED1 = 0; 
unsigned long initialTimeLED2 = 0;  
long intervalLED1 = 1000; // 1 second for SM1 
long intervalLED2 = 2500; // 2.5 seconds for SM2 

const int ledPin1 = 6; 
const int ledPin2 = 7;

int stateLED1 = HIGH; 
int stateLED2 = HIGH;

int enableLED1 = 1; //flag to identify if LED1 should be ON or OFF
int enableLED2 = 1; //flag to identify if LED2 should be ON or OFF

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
  byte btn = MFS.getButton();

    if(btn == BUTTON_1_LONG_PRESSED)
    { 
      MFS.write("1");
      enableLED1 = 0; 
      MFS.writeLeds(LED_1, OFF);
      
    }
    else if(btn == BUTTON_1_LONG_RELEASE)
    { 
      MFS.write("");
      enableLED1 = 1;      
    }

    if(btn == BUTTON_2_LONG_PRESSED)
    { 
      MFS.write("2");
      enableLED2 = 0;
      MFS.writeLeds(LED_2, OFF);
    }
    else if(btn == BUTTON_2_LONG_RELEASE)
    { 
      MFS.write("");
      enableLED2 = 1;
    }

    if(enableLED1)
    {
      //State machine 1 
      if ((millis() - initialTimeLED1) < intervalLED1) 
      { 
        MFS.writeLeds(LED_1, stateLED1);  
      } 
      else 
      { 
        stateLED1 = !stateLED1; 
        initialTimeLED1 = millis();   
      }

    }

    if(enableLED2)
    {
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
  
}