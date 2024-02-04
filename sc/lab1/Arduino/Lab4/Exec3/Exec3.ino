#include <Arduino_FreeRTOS.h>
#include <MultiFuncShield.h> 
#include <TimerOne.h>
#include <semphr.h>

SemaphoreHandle_t binsem;
TaskHandle_t block;

int var_global = 0;

void TaskInc( void *pvParameters );
void TaskDisplay( void *pvParameters );


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  
  //Initialize MFS shield library
  Timer1.initialize();
  MFS.initialize(&Timer1);
  

  xTaskCreate(TaskInc, "Task_S1_Count", 128, NULL, 1, NULL);
  xTaskCreate(TaskDisplay, "Task_S1_display", 128, NULL, 5, &block);



  binsem = xSemaphoreCreateBinary();
  if (binsem != NULL) 
  {
  Serial.println("BinSem created");
  }
  
  vTaskStartScheduler();
}


void TaskInc( void *pvParameters )
{
  while(1)
  {   
    byte btn = MFS.getButton();
    if( btn == BUTTON_1_PRESSED)
    {
      var_global++;
      //MFS.write(var_global);
      xSemaphoreGive(binsem);
      vTaskResume(block);
    }
    vTaskDelay( random(200,500)/ portTICK_PERIOD_MS );
    
  }
}

void TaskDisplay( void *pvParameters)
{  
  while(1)
  {
    if(xSemaphoreTake( binsem, 10) == pdTRUE)
    {
      MFS.write(var_global);

      if( var_global % 10 == 0 && var_global != 0)
      {
        //MFS.beep();
        Serial.print("Multiple of 10 , number -----> ");
        Serial.println(var_global);
      }
      vTaskDelay( random(80,200)/ portTICK_PERIOD_MS );
      vTaskSuspend(block);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
