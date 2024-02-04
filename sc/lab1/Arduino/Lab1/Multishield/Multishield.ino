#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

void setup()
{ 
  Timer1.initialize();
  MFS.initialize(&Timer1); // initialize multi-function shield library
  // Beep control is performed in the background, i.e. beep() is non blocking.
  // short beep for 200 milliseconds
  MFS.beep();
  //Light all LEDs
  MFS.writeLeds(LED_ALL, ON);

  for (int i = 3; i >= 1; i--)
  {
  //Show the result the Display
  MFS.write(i);
  delay(1000);
}
  MFS.write("");
  MFS.writeLeds(LED_ALL, OFF);

  // 4 short beeps, repeated 3 times.
  MFS.beep(5, // beep for 50 milliseconds
          5, // silent for 50 milliseconds
          4, // repeat above cycle 4 times
          3, // loop 3 times
          50 // wait 500 milliseconds between loop
          );
}

void loop() {
//Read the buttons
byte btn = MFS.getButton();
//Present the results
  if (btn == BUTTON_1_PRESSED)
  {
    MFS.write("1");
    MFS.writeLeds(LED_1, ON);
    MFS.writeLeds(LED_2, OFF);
    MFS.writeLeds(LED_3, OFF);
  }
  if (btn == BUTTON_2_PRESSED)
  {
    MFS.write("2");
    MFS.writeLeds(LED_1, OFF);
    MFS.writeLeds(LED_2, ON);
    MFS.writeLeds(LED_3, OFF);
  }
  if (btn == BUTTON_3_PRESSED)
  {
    MFS.write("3");
    MFS.writeLeds(LED_1, OFF);
    MFS.writeLeds(LED_2, OFF);
    MFS.writeLeds(LED_3, ON);
  }
}
























































































 