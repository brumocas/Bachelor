#include <Arduino_FreeRTOS.h>
#include <MultiFuncShield.h>
#include <TimerOne.h>

void TaskBlink1( void *pvParameters );
void TaskPrint( void *pvParameters );
void TaskDispaly( void *pvParameters );
void TaskBuzz( void *pvParameters );

int controlPrint = 1;

void setup() {
  // put your setup code here, to run once:
  //Setup UART
  Serial.begin(9600);

  //Initialize MFS shield library
  Timer1.initialize();
  MFS.initialize(&Timer1); 
  
  /*TaskBlink1 that toggles LED D1 at 1Hz*/
  xTaskCreate( 
              TaskBlink1, // Function to be called
              "Led1",    // Name of task for debuging
              64,        // Stack size
              NULL,       // Parameter to pass to function
              1,          // Task priority (0 to configMAX_PRIORITIES -1)
              NULL);      // Task handle

  /*TaskPrint that counts seconds and sends the value over UART (9600 BaudRate) at 1Hz.*/
  xTaskCreate( TaskPrint, "Print", 64, NULL, 1, NULL );

  /*TaskDisplay that counts the number of clicks on button S1 (left side of the shield), showing the result in the 7-segment display.*/
  xTaskCreate( TaskDisplay, "Countclicks", 64, NULL, 1, NULL);

  /*TaskBuzz that while S3 (right side of the shield) is being pressed, suspends TaskPrint, activates the buzzer and prints “Emergency” in the serial monitor.
  TaskPrint must resume its activity after S3 is released. The S3 should be considered as an emergency button.*/
  xTaskCreate( TaskBuzz, "Emergency_Buzz", 64, NULL, 1, NULL);

  //Start Scheduler that will manage tasks
  vTaskStartScheduler();

}

void TaskBlink1( void *pvParameters ) 
{ 
  pinMode(LED_BUILTIN, OUTPUT);
  while(1)
  {
    MFS.writeLeds(LED_1,ON);
    vTaskDelay(500 / portTICK_PERIOD_MS );
    MFS.writeLeds(LED_1,OFF);
    vTaskDelay(500 / portTICK_PERIOD_MS );
  }
}

void TaskPrint( void *pvParameters )
{
  int counter = 0;
  while(1)
  { 
    if(controlPrint)
    {
      Serial.println(counter);
    }
    counter++;
    vTaskDelay( 1000 / portTICK_PERIOD_MS );  
  }

}

void TaskDisplay (void *pvParameters)
{
  int countClicks = 0;
  while(1)
  {
    byte btn = MFS.getButton();
    if (btn == BUTTON_1_PRESSED)
    {
    countClicks++;
    MFS.write(countClicks);
    }
 }
}

void TaskBuzz( void *pvParameters )
{
  int controleS3 = ON;
  while(1)
  {
    byte btn = MFS.getButton();
    if (btn == BUTTON_3_PRESSED)
    {
      if (controleS3)
      {
        controlPrint=OFF;
        MFS.beep();
        MFS.write("HELP");  
      }
      else
      {
        controlPrint=ON;
        MFS.write(""); 
      }
      controleS3 = !controleS3;
    }
    vTaskDelay (10 / portTICK_PERIOD_MS); 
  }
}
/*possibility to use built in task from FreeRTOS to suspend Task Print

vTaskSuspend ---> Places a task into the Suspended state. A task that is in the Suspended state will never be selected to enter the Running state.
The only way of removing a task from the Suspended state is to make it the subject of a call to vTaskResume().

*/

void loop() {
  // put your main code here, to run repeatedly:
}
