/*
 * Two tasks running in Arduino without preemption (without RTOS)
 * Task 1 runs a typical state machine with 3 states
 *   Blinks the built in LED, with separate intervals ON and OFF (two different states) 
 *   Pressing S(enter) in the serial monitor suspends the blinking  (third state)
 *   Pressing R(enter) in the serial monitor resumes the blinking
 * Pay attention to the state machine pattern with 3 states
 * Task 2 writes the time (millis) to the Serial Monitor each given interval
 */

#define intervalLED1_ON 1000   // LED1 ON time
#define intervalLED1_OFF 1200  // LED1 OFF time
#define intervalWrite 1000   // activation of the write task

unsigned long initialTimeLED1 = 0;

// initialization
void setup() {
  pinMode(13, OUTPUT);
  initialTimeLED1 = millis();
  Serial.begin(9600);
}

// run - task scheduler
void loop() {
  task1();
  task2();  
}

void task1(void){
  static byte state = 0; // state variable of this FSM - static!
       // state == 0 LED ON, during intervalLED1_ON
       // state == 1 LED OFF, during intervalLED1_OFF
       // state == 2 then suspend time count (will not bink)
  static byte next_state, prev_state; // other state variables of this FSM
  static unsigned long currentInterval;
  char a;

  //State 0
  if (state == 0)   // count time with LED ON
  {
    digitalWrite(13, HIGH);
    a=Serial.read();
    
    // transition to state OFF (1)
    if ((millis() - initialTimeLED1) > intervalLED1_ON)
    {
      initialTimeLED1 = millis();  // reset interval count
      next_state = 1;  // next state: switch LED OFF
    }
    // transition to state suspended (2)
    if (a=='S')   // S arrived in the serial port
    {
      next_state = 2;   // next state: suspend time count
      prev_state = state;  // save current state to return to it
      currentInterval = millis() - initialTimeLED1;  // save current count to continue from here
    }
  }
  if (state == 1)    // count time with LED OFF
  {
    digitalWrite(13, LOW);
    a=Serial.read();
   
    // trasition to ON (state 0)
    if ((millis() - initialTimeLED1) > intervalLED1_OFF)
    {
      initialTimeLED1 = millis();  // reset interval count
      next_state = 0;  // next state: switch LED ON
    }
    // transition to SUSPEND (state 2)
    if (a=='S')   // S arrived in the serial port
    {
      next_state = 2;   // next state: suspend time count
      prev_state = state;  // save current state to return to it
      currentInterval = millis() - initialTimeLED1;  // save current count to continue from here
    }
  }
  if (state == 2)   // suspend time counting
  {
    a=Serial.read();
    if (a=='R')   // char received in the serial port
    {
      initialTimeLED1 = millis() - currentInterval;  // recover previous time count
      next_state = prev_state;   // next state: return to time count
    }
  }
  state = next_state;  // evolve to next state as defined by the FSM above
}


void task2(void){
  static long now=0;
  
  if( millis() - now >= intervalWrite){
    // time to write to the serial port
    now=millis(); // reset time reference to count next intervalWrite

    // put here the code to execute every intervalWrite
    Serial.println(now);
  }
}
