  /* Code to execute two approx periodic tasks without RTOS, but using Arduino
   *  Note that the tasks execute **without preemption**
   *  (thus long tasks may cause long blocking to other tasks)
   *  The tasks are continuously called from the Arduino loop()
   *  The control of tasks activation is done by each task separately
   */


const int ledPin = LED_BUILTIN;

void setup() {
  // initialize the pin for the built in LED
  pinMode(ledPin, OUTPUT);
  // initialize the serial port
  Serial.begin(9600);
}


void task1(long interval) {
  static uint8_t state = 0;
  static long now =0;
  
  if (millis() >= now + interval){
    now = millis();  // important for the periodic execution

    // put task1 code here, to run repeatedly every "interval"
    // state machine that blinks the built in LED
    if (state==0){
      digitalWrite(ledPin, LOW);
      state=1;
    }
    else {  // state is 1
      digitalWrite(ledPin, HIGH);
      state=0;
    }
  }
}


void task2(long interval) {
  static float now =0;
  
  if (millis() >= now + interval){
    now = millis();

    // put your main code here, to run repeatedly every "interval"
    Serial.println(now / 1000 ); //Time in seconds ----> now is in miliseconds
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  task1(500);   
  task2(1000);   
}
