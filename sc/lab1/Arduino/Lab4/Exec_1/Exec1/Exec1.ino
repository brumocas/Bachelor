#include <Arduino_FreeRTOS.h>
#include <MultiFuncShield.h> 
#include <TimerOne.h>
#include <semphr.h>

SemaphoreHandle_t mutex;

int var_global = 0;

void TaskInc( void *pvParameters );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  
  //Initialize MFS shield library
  Timer1.initialize();
  MFS.initialize(&Timer1); 
  

  xTaskCreate(TaskInc, "Task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskInc, "Task2", 128, NULL, 1, NULL);

  /**
   Create mutex
   https://www.freertos.org/a00122.html
  */
  mutex = xSemaphoreCreateMutex();
  if (mutex != NULL) 
  {
  Serial.println("Mutex created");
  }
  
  vTaskStartScheduler();
}


void TaskInc( void *pvParameters )
{
  int var_local = var_global;
  while(1)
  { 
    if (xSemaphoreTake(mutex, 10) == pdTRUE)
    {
      var_local++;
      vTaskDelay( random(500,1000)/ portTICK_PERIOD_MS );
      var_global = var_local;
      Serial.println(var_global);
 
      MFS.write(var_global);
      
      /**
         Give mutex
         https://www.freertos.org/a00123.html
      */
      xSemaphoreGive(mutex);
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
