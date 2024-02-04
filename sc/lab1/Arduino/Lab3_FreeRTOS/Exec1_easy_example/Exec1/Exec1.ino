#include <Arduino_FreeRTOS.h>
#include <MultiFuncShield.h>

void TaskBlink1( void *pvParameters );

const int TIME_ON = 1000 ;
const int TIME_OFF = 100;

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

  //Start Scheduler that will manage tasks
  vTaskStartScheduler();
}

void TaskBlink1( void *pvParameters ) 
{ 
  pinMode(LED_BUILTIN, OUTPUT);
  while(1)
  {
    digitalWrite(LED_BUILTIN,HIGH);
    vTaskDelay( TIME_ON / portTICK_PERIOD_MS );
    digitalWrite(LED_BUILTIN,LOW);
    vTaskDelay( TIME_OFF / portTICK_PERIOD_MS );
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
