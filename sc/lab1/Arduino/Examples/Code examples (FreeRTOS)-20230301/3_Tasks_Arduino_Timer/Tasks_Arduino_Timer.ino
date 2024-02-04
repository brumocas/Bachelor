/*
 * Tasks launched in Arduino, without preemption, but with timer 1 setting a regular time base - TICK
 * Two tasks, one blinks a LED the other prints time to the serial monitor
 */


#include <TimerOne.h>

#define TIME1 70   // period of Task_LED in TICKs
#define TIME2 100  // period of Task_Serial in TICKs
#define TICK 10000  // TICK in microseconds

const int ledPin = 13; // define pin of built in LED to output

void setup()
{
  pinMode (ledPin, OUTPUT); // set the pin to output

  Serial.begin(9600);

  Timer1.initialize(TICK); // initialize Timer 1 to count "TICK" in microseconds
  Timer1.attachInterrupt(taskSched);  // attach an interrupt service routing to the timer (1 interrupt every TICK us)
}

void loop(){}

//Function called by the Timer as an interruption
// used to schedule the tasks
void taskSched(void){
  task_LED();
  task_Serial();
}


////////////////
// task_LED
// toggles LED every TIME1 ticks (SW timer 1)
void task_LED(void) {
  static int time1=TIME1;
  static byte LEDstate=HIGH;
  
  if(time1)
    time1--; // if time1 not 0, decrement
  else {
    time1 = TIME1; // reset to SW timer 1
    // do here the task code, to run every TIME1 ticks
    LEDstate = !LEDstate;
    digitalWrite(ledPin,LEDstate);
  }
}


////////////////
// task_Serial
// Writes current time in ms to the serial monitor every TIME2 ticks (SW timer 2)
void task_Serial(void) {
  static int time2=TIME2;

  if(time2)
    time2--; // if time2 not 0, decrement
  else {
    time2 = TIME2; // reset to SW timer 2
    // do here the task code, to run every TIME2 ticks
    Serial.println(millis());
  }
}
