#include <Arduino_FreeRTOS.h>
#include <MultiFuncShield.h>
#include <TimerOne.h>

void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );

void setup() {
  // put your setup code here, to run once:
  //Setup UART
  Serial.begin(9600);

  //Initialize MFS shield library
  Timer1.initialize();
  MFS.initialize(&Timer1); 
  
  //Create task to toogle  LED D1
  xTaskCreate( 
              TaskBlink1, // Function to be called
              "Blink1",    // Name of task for debuging
              128,        // Stack size
              NULL,       // Parameter to pass to function
              1,          // Task priority (0 to configMAX_PRIORITIES -1)
              NULL);      // Task handle

  //Create task to toogle  LED D2
  xTaskCreate( TaskBlink2, "Blink2", 128, NULL, 1, NULL );

  //Start Scheduler that will manage tasks
  vTaskStartScheduler();

}

void TaskBlink1( void *pvParameters ) 
{ 
  pinMode(LED_BUILTIN, OUTPUT);
  while(1)
  {
    MFS.writeLeds(LED_1,ON);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    MFS.writeLeds(LED_1,OFF);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}

void TaskBlink2( void *pvParameters ) 
{ 
  pinMode(LED_BUILTIN, OUTPUT);
  while(1)
  {
    MFS.writeLeds(LED_2,ON);
    vTaskDelay( 2500 / portTICK_PERIOD_MS );
    MFS.writeLeds(LED_2,OFF);
    vTaskDelay( 2500 / portTICK_PERIOD_MS );
  }
}


void loop() {
  // put your main code here, to run repeatedly:
}