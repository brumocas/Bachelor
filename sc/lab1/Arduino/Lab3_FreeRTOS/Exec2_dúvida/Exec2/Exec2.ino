#include <Arduino_FreeRTOS.h>
#include <MultiFuncShield.h>
#include <TimerOne.h>
#include <semphr.h>

void TaskBlink1( void *pvParameters );
void TaskPrint( void *pvParameters );
void TaskDispaly( void *pvParameters );
void TaskBuzz( void *pvParameters );

TaskHandle_t Handle_Print;
TaskHandle_t Handle_Count;
SemaphoreHandle_t mutex;

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
              128,        // Stack size
              NULL,       // Parameter to pass to function
              1,          // Task priority (0 to configMAX_PRIORITIES -1)
              NULL);      // Task handle

  /*TaskPrint that counts seconds and sends the value over UART (9600 BaudRate) at 1Hz.*/
  xTaskCreate( TaskPrint, "Print", 128, NULL, 1, &Handle_Print);

  /*TaskDisplay that counts the number of clicks on button S1 (left side of the shield), showing the result in the 7-segment display.*/
  xTaskCreate( TaskDisplay, "Countclicks", 128, NULL, 2, &Handle_Count);

  /*TaskBuzz that while S3 (right side of the shield) is being pressed, suspends TaskPrint, activates the buzzer and prints “Emergency” in the serial monitor.
  TaskPrint must resume its activity after S3 is released. The S3 should be considered as an emergency button.*/
  xTaskCreate( TaskBuzz, "Emergency_Buzz", 128, NULL, 2, NULL);
  
  Serial.println("Program started..........");
  
  mutex = xSemaphoreCreateMutex();

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
    counter++;
    Serial.println(counter);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}


void TaskDisplay (void *pvParameters)
{
  int countClicks = 0;
  while(1)
  { 
    if(xSemaphoreTake( mutex , 10 ) == pdTRUE)
    {
      byte btn = MFS.getButton();
      if (btn == BUTTON_1_PRESSED)
      {
        countClicks++;
        MFS.write(countClicks);
      }

      vTaskDelay( 100/ portTICK_PERIOD_MS );
      xSemaphoreGive(mutex);
      vTaskSuspend(Handle_Count);
    }
  }
}


void TaskBuzz( void *pvParameters )
{
  while(1)
  { 

    if(xSemaphoreTake( mutex, 10) == pdTRUE)
    {
      byte btn = MFS.getButton();
      if(btn == BUTTON_3_PRESSED)
      { 
        Serial.println("Emergency");
        //MFS.beep();
      }
      vTaskDelay( 100/ portTICK_PERIOD_MS );
      xSemaphoreGive(mutex);
      vTaskResume(Handle_Count);
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
}