#include <Arduino_FreeRTOS.h>
#include <TimerOne.h>
#include <MultiFuncShield.h>

//Tasks
void taskBlink1 (void *pvParameters);
void taskPrint (void *pvParameters);
void taskDisplay (void *pvParameters);
void taskBuzz (void *pvParameters);

int controlPrint = ON;

//Initialization
void setup() {
  Timer1.initialize(); 
  MFS.initialize(&Timer1);

  Serial.begin(9600);

  xTaskCreate (taskBlink1, //Function
              "Led1",      //Description
              128,         //Stack size in words
              NULL,        //Parameters for the function
              1,           //Priority
              NULL);       //Task handler*/

  xTaskCreate (taskPrint, "Print", 128, NULL, 1, NULL);
  xTaskCreate (taskDisplay, "Display", 128, NULL, 1, NULL);
  xTaskCreate (taskBuzz, "Buzz", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void taskBlink1 (void *pvParameters)
{
  while(1)
  {
    MFS.writeLeds (LED_1, ON);
    vTaskDelay (500 / portTICK_PERIOD_MS);
    MFS.writeLeds (LED_1, OFF);
    vTaskDelay (500 / portTICK_PERIOD_MS);
  }
}

void taskPrint (void *pvParameters)
{
  int counter = 0;
  while(1)
  {
    if (controlPrint)
    { 
      Serial.println(counter);  
    }
    counter++;
    vTaskDelay (1000 / portTICK_PERIOD_MS); 
  }
}

void taskDisplay (void *pvParameters)
{
  int countClicks = 0;
  while(1)
  {
    byte btn = MFS.getButton();
    if (btn == BUTTON_1_PRESSED)
    {
      //Show the number of times Button 1 was clicked
      countClicks++;
      MFS.write(countClicks); 
    }
    vTaskDelay (10 / portTICK_PERIOD_MS); 
  }
}

void taskBuzz (void *pvParameters)
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

void loop() {
}
