 /*
  *  Code with two tasks that show their execution printing a char in the serial monitor
  *  Objective: to observe **preemption**
  *  Try different priorities: 
  *    Task 1 -> priority 1 + Task 2 -> priority 2
  *    Task 1 -> priority 2 + Task 2 -> priority 1
  *    Task 1 -> priority 1 + Task 2 -> priority 1
  *    Task 1 -> priority 1 + Task 2 -> priority 5
  *  explain what you observe
  */


// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

void delay_arduino( unsigned int long ms); 


void setup() {

  Serial.begin(9600);

  /**
     Create tasks
  */
  xTaskCreate(Task1, // Task function
              "Task1", // Task name for humans
              128, 
              (void*)1000, // Task parameter = cyclic delay
              1, // Task priority
              NULL);

  xTaskCreate(Task2, "Task2", 128, (void*)1290, 2, NULL);
  
  Serial.println("Program started......");
  
  vTaskStartScheduler(); //Put the Task in the queue by priority  
}

void loop() {}

void Task1(void *pvParameters)
{
  int period = (int)pvParameters; // Use task parameters to define delay
  
  while(1)
  { 
    Serial.print("B");
    for (int i=0; i<10; i++) {
      Serial.print("b");
      delay_arduino(100);
    }
    Serial.println("B");
    
    vTaskDelay(period / portTICK_PERIOD_MS);
    
  }
}

void Task2(void *pvParameters)
{
  int period = (int)pvParameters; // Use task parameters to define delay
  
  while(1)
  {
    Serial.print("A");
    for (int i=0; i<4; i++) {
      Serial.print("a");
      delay_arduino(100);
    }
    Serial.println("A");
    
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
