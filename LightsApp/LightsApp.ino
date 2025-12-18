#include "FastLED.h"
#include "LED_Buffer.h"
#include <queue>

CRGB shownLeds[NUM_LEDS];
LED_Buffer led_buffer;

TaskHandle_t Task1;
TaskHandle_t Task2;

// --- SPEED CONTROL ---
// This controls how many milliseconds wait between animation updates.
// 30ms is roughly 33 frames per second (Smooth)
// 100ms is 10 frames per second (Choppy/Slow)
#define ANIMATION_SPEED_MS 30 

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, 26, RGB>(shownLeds, NUM_LEDS);
  Serial.begin(9600);
  
  InitTwinkleMode(); // Initialize the Christmas Effect

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
    
    // This controls how often we SEND data to the strip (Refresh Rate)
    // 17ms is approx 60hz. This is fine to leave as is.
    if (now - last > 17) 
    {
      if(led_buffer.IsBufferReadyToBeWritten()) {
        last = now;
        memcpy(shownLeds, led_buffer.getWriteArrayFromBuffer(), NUM_LEDS*sizeof(CRGB));
        FastLED.show();
        led_buffer.ClearAndIncrementWrittenIndex();
      }
      
    }
    // Good practice in RTOS tasks to prevent watchdog starvation if the if-statement isn't met
    delay(1); 
  } 
}

//Task2code: Calculates the animation logic
void Task2code( void * pvParameters ){

  unsigned long lastStepTime = 0;

  for(;;){
    unsigned long now = millis();

    // THIS is the new speed governor
    if (now - lastStepTime > ANIMATION_SPEED_MS) {
      
      if(led_buffer.IsBufferReadyToBeUpdated()) {
        lastStepTime = now;
        
        StepTwinkle(led_buffer);
        
        // StepSpiral(led_buffer);
        // Step_SpiralTrails(led_buffer);
        // Step_SpiralRainbow(led_buffer);
        // Step_CandyCaneSpiral(led_buffer);
      }
    }
    
    // Essential for ESP32: Yield to the OS so background tasks (WiFi/Bluetooth/System) don't crash
    delay(1); 
  }
}

void loop() {
  // Main loop is empty because we are using RTOS Tasks
}