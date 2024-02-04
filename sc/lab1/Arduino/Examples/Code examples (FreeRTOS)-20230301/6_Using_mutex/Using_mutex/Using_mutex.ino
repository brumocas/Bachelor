/////////////////////////////
//  This example shows 2 tasks that share a global variable, incrementing it
//  The tasks print the global variable value before and after incrementing
//  There is a delay() (busy wait) between printing the global variable and
//          incrementing it -- this delay increases the chances of preemption
//  When there is preemption, the printing of the sequential value is corrupted
//  ** check with and without the mutex **


// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>


// Include mutex support
#include <semphr.h>

/*
   Declaring a global variable of type SemaphoreHandle_t
*/
SemaphoreHandle_t mutex;

int globalCount = 0;

void delay_arduino( unsigned int long ms);

void setup() {

  Serial.begin(9600);
  
  Serial.println("");
  Serial.println("Program Starting........");

  /**
       Create a mutex.
       https://www.freertos.org/CreateMutex.html
  */  
  mutex = xSemaphoreCreateMutex();
  if (mutex != NULL) {
    Serial.println("Mutex created");
  }


  /**
     Create tasks
  */
  xTaskCreate(Task1, // Task function
              "Task1", // Task name for humans
              128, 
              (void*)800, // Task parameter - period in ms
              1, // Task priority
              NULL);

  xTaskCreate(Task2, "Task2", 128, (void*)955, 2, NULL); 
  // different periods to create different interference moments

  

  vTaskStartScheduler();
}

void loop() {}


void Task1(void *pvParameters)
{
  int period = (int)pvParameters; // Use task parameters to define approx period

  for (;;)
  {
    /**
       Take mutex
       https://www.freertos.org/a00122.html
    */
    if (xSemaphoreTake(mutex, 10) == pdTRUE)
    {
      Serial.print(pcTaskGetName(NULL)); // Get task name
      Serial.print(", Count read value: ");
      Serial.print(globalCount);

      delay_arduino(100); // busy wait to increase chances of preemption here
      globalCount++;

      Serial.print(", Updated value: ");
      Serial.print(globalCount);

      Serial.println();
      /**
         Give mutex
         https://www.freertos.org/a00123.html
      */
      xSemaphoreGive(mutex);
    }

    vTaskDelay(period / portTICK_PERIOD_MS);
  }
}



void Task2(void *pvParameters)
{
  int period = (int)pvParameters; // Use task parameters to define delay

  for (;;)
  {
    /**
       Take mutex
       https://www.freertos.org/a00122.html
    */
    if (xSemaphoreTake(mutex, 10) == pdTRUE)
    {
      Serial.print(pcTaskGetName(NULL)); // Get task name
      Serial.print(", Count read value: ");
      Serial.print(globalCount);

      delay_arduino(100);
      globalCount++;

      Serial.print(", Updated value: ");
      Serial.print(globalCount);

      Serial.println();
      /**
         Give mutex
         https://www.freertos.org/a00123.html
      */
      xSemaphoreGive(mutex);
    }

    vTaskDelay(period / portTICK_PERIOD_MS);
  }
}

void delay_arduino(unsigned long ms)
{
	uint32_t start = micros();

	while (ms > 0) {
		yield();
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}
