#include <Arduino_FreeRTOS.h>

void TaskBlink1( void *pvParameters );
void TaskPrint( void *pvParameters );

TaskHandle_t Handle_print; //pointer to suspend TaskPrint in other Task code 

void setup() {
  // put your setup code here, to run once:
  //Setup UART
  Serial.begin(9600);

  //Create task to toogle  LED D1
  xTaskCreate( 
              TaskBlink1, // Function to be called
              "Task1",    // Name of task for debuging
              128,        // Stack size
              NULL,       // Parameter to pass to function
              1,          // Task priority (0 to configMAX_PRIORITIES -1)
              NULL);      // Task handle

  //Create Task for Serial Coms
  xTaskCreate( TaskPrint, "Task3", 128, NULL, 1, &Handle_print );

  //Start Scheduler that will manage tasks
  vTaskStartScheduler();
}

void TaskBlink1( void *pvParameters ) 
{ 
  pinMode(LED_BUILTIN, OUTPUT);
  while(1)
  { 
    vTaskSuspend( Handle_print );
    digitalWrite(LED_BUILTIN,HIGH);
    vTaskDelay(5000 / portTICK_PERIOD_MS );
    vTaskResume( Handle_print );
    digitalWrite(LED_BUILTIN,LOW);
    vTaskDelay(5000 / portTICK_PERIOD_MS );
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

void loop() {
  // put your main code here, to run repeatedly:

}
