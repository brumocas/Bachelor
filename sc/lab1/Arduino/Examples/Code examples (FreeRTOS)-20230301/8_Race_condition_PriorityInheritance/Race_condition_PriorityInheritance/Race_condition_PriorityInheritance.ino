  /*
   *   Two tasks compete to write to the serial monitor (race condition)
   *   One writes BbbbbB (higher priority) and the other AaaaaaaaaaaA (lower priority)
   *   Their critical regions are bbbb and aaaaaaaaaa
   *   Use semaphores (mutex and binary) to synchronize the two tasks and protect the critical regions
   *   A third task has intermediate priority and writes CccccccccC to the serial monitor
   *   It does not use the semaphor
   *   1-Run this set without semaphores and see that all patterns interfere according to priorities
   *   2-Run this set with a mutex and see that the patterns "a" and "b" do not interfere
   *      In particular see that the pattern "c" does not block "b" (the highest priority task)
   *   3-Run this set with a binary semaphore and see that the patterns "a" and "b" do not interfere, either
   *      However, now "b" can be blocked by "c" increasing its blocking time (priority inversion)
   */  

// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>

// Include mutex support
#include <semphr.h>

void delay_arduino(unsigned long ms);
 
/*
   Declaring a global variable of type SemaphoreHandle_t
   as well as a shared buffer
*/
SemaphoreHandle_t binSem;
char buffer[10];

void setup() {

  Serial.begin(9600);

  binSem = xSemaphoreCreateMutex();   // uncomment one or the other or both
  //binSem = xSemaphoreCreateBinary();
  xSemaphoreGive(binSem);  // use this line just with binary semaphores (they are created locked)
  if (binSem != NULL) {   // comment if not using semaphores
    Serial.println("Semaphore created");
  }

  /**
     Create tasks
  */
  xTaskCreate(Task1, // Task function
              "Task1", // Task name for humans
              128, 
              (void*)255, // Task parameter - cylic idle delay
              3, // Task priority
              NULL);

  xTaskCreate(Task2, "Task2", 128, (void*)385, 1, NULL);
  xTaskCreate(Task3, "Task3", 128, (void*)315, 2, NULL);
  // different cyclic delays to interfere in different moments
}

void loop() {}


/* 
 * This task writes 'BbbbbB' to the serial monitor, slowly - 20ms per char (with delay()) 
 * The critical region is just the lower case letter 'bbbb'
 */

void Task1(void *pvParameters)
{
  int period = (int)pvParameters; // Use task pvParameters to define idle time

  while(1)
  {
    Serial.print("B");
    if (xSemaphoreTake(binSem, 1000/ portTICK_PERIOD_MS) == pdTRUE)
    { // this is the critical section
      for (int i=0; i<4; i++) {
        Serial.print("b");
        delay_arduino(20);
      }
      xSemaphoreGive(binSem);
    }  // end of the cricial section
    else
      Serial.print(".");  // if there is a timeout while blocked in Take
    Serial.println("B");

    vTaskDelay(period / portTICK_PERIOD_MS);  // go to idle mode until next activation
  }
}


/* 
 * This task writes 'AaaaaaaaaaaA' to the serial monitor, slowly - 20ms per char (with delay()) 
 * The critical region is just the lower case letter 'aaaaaaaaaa'
 */

void Task2(void *pvParameters)
{
  int period = (int)pvParameters; // Use task parameters to define delay

  while(1)
  {
    Serial.print("A");
    if (xSemaphoreTake(binSem, 1000/ portTICK_PERIOD_MS) == pdTRUE)
    { // this is the critical section
      for (int i=0; i<10; i++) {
        Serial.print("a");
        delay_arduino(20);
      }
      xSemaphoreGive(binSem);
    }  // end of the cricial section
    else
      Serial.print(".");  // if there is a timeout while blocked in Take
    Serial.println("A");

    vTaskDelay(period / portTICK_PERIOD_MS);
  }
}



/* 
 * This task does not use the semaphore (no critical region) and has intermediate priority 
 * It writes 'CccccccccC' to the serial monitor, slowly - 20ms per char (with delay()) 
 */

void Task3(void *pvParameters)
{
  int period = (int)pvParameters; // Use task parameters to define delay

  while(1)
  {
    Serial.print("C");
    for (int i=0; i<8; i++) {
      Serial.print("c");
      delay_arduino(20);
    }
    Serial.println("C");

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
