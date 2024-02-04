#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <MultiFuncShield.h>
#include <TimerOne.h>

int sharedBuffer = 0;

void TaskProd (void *pvParameters);
void TaskCons (void *pvParameters);

SemaphoreHandle_t binSem;
TaskHandle_t prod;

void setup() {

  Serial.begin (9600);

  xTaskCreate (TaskProd, "T1", 128, NULL, 1, &prod);
  xTaskCreate (TaskCons, "T2", 128, NULL, 1, NULL);

  binSem = xSemaphoreCreateBinary();

  vTaskStartScheduler();
}

void TaskProd (void *pvParameters) {
  while(1) { 
    if (sharedBuffer < 1)
      sharedBuffer++;

    Serial.print(pcTaskGetName(xTaskGetCurrentTaskHandle())); 
    Serial.print(": Buffer is "); 
    Serial.println(sharedBuffer);

    vTaskDelay (random(100,500)/ portTICK_PERIOD_MS);
    
    //Release the binary semaphore
    xSemaphoreGive(binSem);  
    vTaskSuspend(prod);
  } 
}

void TaskCons (void *pvParameters) {
  while(1) { 
    //Take the binary sempahore
    if(xSemaphoreTake(binSem, portMAX_DELAY) == pdTRUE)
    { 
      //if (sharedBuffer > 1)
        sharedBuffer--;
      
      Serial.print(pcTaskGetName(xTaskGetCurrentTaskHandle())); 
      Serial.print(": Buffer is "); 
      Serial.println(sharedBuffer);

      //Serial.print(Serial.readString()); Ler string da consola e imprimir 
      
      //Release the binary semaphore
      vTaskDelay (random(500,1000)/ portTICK_PERIOD_MS);
      
      //Executar uma a seguir  outra
      vTaskResume(prod);
    }
  }  
}

void loop() {
  
}
