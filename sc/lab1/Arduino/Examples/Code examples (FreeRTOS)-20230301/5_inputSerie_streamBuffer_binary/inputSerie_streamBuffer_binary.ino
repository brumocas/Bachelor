  /*
   *   Using the serial read to replace a button (e.g. if you just have the plain Arduino)
   *   Two tasks: 
   *   one periodic, reads the serial input (like reading a button), activates the other one
   *   one sporadic, activated by the previous one when there is an input
   *   Activation is done with a binary semaphore
   *   Communication with a Stream Buffer
   */  

// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stream_buffer.h>
 
/*
   Declaring global variables: binary semaphore and stream buffer
*/
SemaphoreHandle_t binSem;
StreamBufferHandle_t streamBuf;


void setup() {

  Serial.begin(9600);

  binSem = xSemaphoreCreateBinary();
  if (binSem != NULL) {
    Serial.println("Semaphore created");
  }

  streamBuf = xStreamBufferCreate(10,1);
  if (streamBuf != NULL) {
    Serial.println("Stream Buffer created");
  }


  /**
     Create tasks
  */
  xTaskCreate(ReadInput, // Task function
              "T_Read", // Task name for humans
              128, 
              NULL, // Task parameter - no parameter
              2, // Task priority
              NULL);

  xTaskCreate(ProcessInput, "T_Process", 128, NULL, 1, NULL);

  Serial.println("Tasks created");
}

void loop() {}


/* 
 * This task periodically polls the serial input (from the serial monitor) 
 * If something received, then give the semaphore to wake up the processing task 
 */

void ReadInput(void *pvParameters){
  char readByte;
  
  while(1){
    readByte = Serial.read();
    if (readByte >= 0){
      // actual byte was received  (-1 if nothing received) 
      xStreamBufferSend(streamBuf, &readByte, 1, 10);  //send just 1 byte at a time
      xSemaphoreGive(binSem);  //  give the semaphore to activate the other task
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);  // go to idle mode until next activation
  }
}


/* 
 * This task is woken up by the other task when a byte is received using a semaphore
 * writes the byte to serial port
 */

void ProcessInput(void *pvParameters){
  char readByte;
  
  while(1){
    if (xSemaphoreTake(binSem, 1000/ portTICK_PERIOD_MS) == pdTRUE){
      // byte received
      xStreamBufferReceive(streamBuf, &readByte, 1, 10);
      Serial.print(readByte);
    }
    else{
      // the semaphore did not unblock after the timeout (1000ms), print '.'
      Serial.print(".");
    }
    // remember the vTaskDelay is not needed because activation is controlled by the semaphore
  }
}
