#include "FastLED.h"
#include "LED_Buffer.h"
#include <queue>

CRGB shownLeds[NUM_LEDS];
LED_Buffer led_buffer;

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, 26, RGB>(shownLeds, NUM_LEDS);
  Serial.begin(9600);
  InitTwinkleMode();
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
    disableCore0WDT();
    disableCore1WDT();
    
  
}


void Task1code( void * pvParameters ){
  
  unsigned long last = millis();
  for(;;){

    unsigned long now = millis();
    
    if (now - last > 17) // 20ms = 50Hz
    {
      if(led_buffer.IsBufferReadyToBeWritten()) {
        last = now;
        memcpy(shownLeds, led_buffer.getWriteArrayFromBuffer(), NUM_LEDS*sizeof(CRGB));
        FastLED.show();
        led_buffer.ClearAndIncrementWrittenIndex();
      }
      
    }
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){


  for(;;){
    if(led_buffer.IsBufferReadyToBeUpdated()) {
      StepTwinkle(led_buffer);
      //StepSpiral(led_buffer);
//      Step_SpiralTrails(led_buffer);
//      Step_SpiralRainbow(led_buffer);
//        Step_CandyCaneSpiral(led_buffer);
    }
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  uint16_t i;

//  for(i = 0; i < NUM_LEDS; i++) {
//    if((i%3) == 0) {
//      leds[i] = CRGB::Black;
//    } else if((i%3) == 1) {
//      leds[i] = CRGB::Black;
//    } else {
//      leds[i] = CRGB::Black;
//    }
//  }
//  StepTwinkle();
//  FastLED.show();
//    delay(100);
//    Serial.print("loop() running on core ");
//    Serial.println(xPortGetCoreID());
}
