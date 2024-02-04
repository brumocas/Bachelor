  /*
   *   Two tasks compete to write to a shared buffer (race condition)
   *   First, run the tasks without synchronization (no semaphores) and observe the buffer inconsistencies
   *   Use semaphores (mutex and binary) to synchronize the two tasks
   *   Second, create a mutex and protect the critical section
   *   Third, instead of a mutex, now create a binary semaphore to enforce
   *   precedence (one task is periodic and activates the other when finishing its cycle
   *   Note that enforcing precedences in the tasks execution also prevents the race condition
   *   Compile and run each case
   */  

// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>

// Include mutex support
#include <semphr.h>

void delay_arduino( unsigned long ms);
 
/*
   Declaring a global variable of type SemaphoreHandle_t
   as well as a shared buffer
*/
SemaphoreHandle_t binSem = NULL;
char buffer[10];

void setup() {

  Serial.begin(9600);

  //binSem = xSemaphoreCreateMutex();   // uncomment one or the other
  binSem = xSemaphoreCreateBinary();
  
  
  if (binSem != NULL) {               // also uncomment this part
    Serial.println("Semaphore created");
  }
  
  

  /**
     Create tasks
  */
  xTaskCreate(Task1, // Task function
              "Task1", // Task name for humans
              128, 
              (void*)255, // Task parameter - cylic idle delay
              2, // Task priority
              NULL);

  xTaskCreate(Task2, "Task2", 128, (void*)385, 1, NULL);
  // different cyclic delays to interfere in different moments
}

void loop() {}


/* 
 * This task write 8 'B' to the buffer, slowly - 25ms per char! 
 */

void Task1(void *pvParameters)
{
  int period = (int)pvParameters; // Use task pvParameters to define idle time

  while(1)
  {
    /*
     *   uncomment to take the semaphore (mutex case, only)
    */
    
    if (xSemaphoreTake(binSem, 100/ portTICK_PERIOD_MS) == pdTRUE)
    { // this is the critical section
      Serial.print(pcTaskGetName(NULL)); // Get and write task name (defined when created)
      Serial.print(": ");
      for (int i=0; i<8; i++) {
        buffer[i]='B';
        delay_arduino(25);
      }
      for (int i=0; i<8; i++) Serial.print(buffer[i]);
      Serial.println("");

      /*
       *  uncomment to give the semaphore (mutex or the binary semaphore cases)
      */
      //xSemaphoreGive(binSem);
    }  // end of the cricial section

    vTaskDelay(period / portTICK_PERIOD_MS);  // go to idle mode until next activation
  }
}


/* 
 * This task write 10 'A' to the buffer, slowly - 20ms per char! 
 */

void Task2(void *pvParameters)
{
  int period = (int)pvParameters; // Use task parameters to define delay

  while(1)
  {
    /*
     *   uncomment the if to take the semaphore (both mutex or binary semaphore cases)
    */
    //if (xSemaphoreTake(binSem, 100/ portTICK_PERIOD_MS) == pdTRUE)
    { // this is the critical section
      Serial.print(pcTaskGetName(NULL)); // Get and write task name
      Serial.print(": ");
      for (int i=0; i<10; i++) {
        buffer[i]='A';
        delay_arduino(20);
      }
      for (int i=0; i<10; i++) Serial.print(buffer[i]);
      Serial.println(" ");

      /*
       *  uncomment to give the semaphore (just mutex case)
      */
      xSemaphoreGive(binSem);
    }  // end of the cricial section

    /*
     * if using the binary semaphore the task cycle execution 
     * is controlled by semaphore take (task activated when the
     * semaphore is given
     * --> thus this is not needed, comment in the binary semaphore case
     */
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
