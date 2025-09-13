#include "LED_Buffer.h"

class Trailing_LED {
  public:
    int16_t currBrightness;
    int16_t currHue;
};

CRGB Spiral_LED_Array[NUM_LEDS];

Trailing_LED Trailing_LEDs[NUM_LEDS]; 



uint8_t spiralHue = 200;
uint8_t spiralSaturation = 150;
uint8_t spiralBrightness = 254;

void StepSpiral(LED_Buffer &led_buffer) {
  static int16_t currIndex;
  const uint16_t StepFrequency = 3; // Steps every 1 step
  static uint16_t cnt = 0;
  static bool bool_SpiralingForward = true;
  uint16_t i;
  
  cnt++;
  if(cnt > StepFrequency) {

    if(bool_SpiralingForward) {
      currIndex++;
      if(currIndex == NUM_LEDS) {
          bool_SpiralingForward = false;
          currIndex--;
      }
      Spiral_LED_Array[currIndex] = CHSV(spiralHue, spiralSaturation, spiralBrightness);
    } else {
      Spiral_LED_Array[currIndex] = 0;
      currIndex--;
      if(currIndex < 0) {
        currIndex = 0;
        bool_SpiralingForward = true;
      }
      Spiral_LED_Array[currIndex] = CHSV(spiralHue, spiralSaturation, spiralBrightness);
    }    
    cnt = 0;
  }
  
  led_buffer.updateBufferWithArray(Spiral_LED_Array);
  led_buffer.SetAndIncrementUpdateIndex();
}


void Step_SpiralTrails(LED_Buffer &led_buffer) {
  static int16_t currIndex;
  const uint16_t StepFrequency = 3; // Steps every 1 step
  static uint16_t cnt = 0;
  static bool bool_SpiralingForward = true;
  uint16_t i;
  
  cnt++;
  if(cnt > StepFrequency) {

    if(bool_SpiralingForward) {
      Spiral_LED_Array[currIndex] = 0;
      currIndex++;
      if(currIndex == NUM_LEDS) {
          bool_SpiralingForward = false;
          currIndex--;
      }
      Spiral_LED_Array[currIndex] = CHSV(spiralHue, spiralSaturation, spiralBrightness);
    } else {
      Spiral_LED_Array[currIndex] = 0;
      currIndex--;
      if(currIndex < 0) {
        currIndex = 0;
        bool_SpiralingForward = true;
      }
      Spiral_LED_Array[currIndex] = CHSV(spiralHue, spiralSaturation, spiralBrightness);
    }   
    for(i = 0; i < NUM_LEDS; i++) {
      if( (Trailing_LEDs[i].currBrightness > 0) && (i != currIndex) ) {
        Trailing_LEDs[i].currBrightness = Trailing_LEDs[i].currBrightness - 10;
        if(Trailing_LEDs[i].currBrightness < 0) {
          Trailing_LEDs[i].currBrightness = 0;
        }
        Spiral_LED_Array[i] = CHSV(spiralHue, spiralSaturation, Trailing_LEDs[i].currBrightness);
      }
    }
    Trailing_LEDs[currIndex].currBrightness = spiralBrightness; 
    cnt = 0;
  }

  
  led_buffer.updateBufferWithArray(Spiral_LED_Array);
  led_buffer.SetAndIncrementUpdateIndex();

}

void Step_SpiralRainbow(LED_Buffer &led_buffer) {
  static int16_t currIndex;
  const uint16_t StepFrequency = 0; // Steps every 1 step
  static uint16_t cnt = 0;
  static bool bool_SpiralingForward = true;
  uint16_t i;
  const uint8_t DELTA_HUE = 6;
  spiralSaturation = 255;
  cnt++;
  if(cnt > StepFrequency) {

    if(bool_SpiralingForward) {
      currIndex++;
      spiralHue = spiralHue + DELTA_HUE;
      if(currIndex == NUM_LEDS) {
          bool_SpiralingForward = false;
          spiralHue = spiralHue - DELTA_HUE;
          currIndex--;
      }
      Spiral_LED_Array[currIndex] = CHSV(spiralHue, spiralSaturation, spiralBrightness);
    } else {
      Spiral_LED_Array[currIndex] = 0;
      currIndex--;
      spiralHue = spiralHue - DELTA_HUE;
      if(currIndex < 0) {
        currIndex = 0;
        bool_SpiralingForward = true;
        spiralHue = spiralHue + DELTA_HUE;
      }
      Spiral_LED_Array[currIndex] = CHSV(spiralHue, spiralSaturation, spiralBrightness);
    }   
    cnt = 0;
  }

  
  led_buffer.updateBufferWithArray(Spiral_LED_Array);
  led_buffer.SetAndIncrementUpdateIndex();

}

void Step_CandyCaneSpiral(LED_Buffer &led_buffer) {
   static int16_t offset = 0; // Offset to simulate cycling pattern
  const uint16_t StepFrequency = 3; // Adjust speed of cycling
  static uint16_t cnt = 0;

  // Define candy cane colors
  const CHSV colorRed = CHSV(0, 255, 255); // Red
  const CHSV colorWhite = CHSV(0, 0, 255); // White
  
  cnt++;
  if (cnt > StepFrequency) {
    // Cycle the pattern by incrementing the offset
    offset = (offset + 1) % NUM_LEDS; 
    cnt = 0;
  }

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    // Determine which color to use based on the block position
    uint16_t virtualIndex = (i + offset) % (10); // 10 LEDs per full red+white cycle (5 each)
    
    if (virtualIndex < 5) {
      Spiral_LED_Array[i] = colorRed; // First 5 LEDs are red
    } else {
      Spiral_LED_Array[i] = colorWhite; // Next 5 LEDs are white
    }
  }

  // Update LED buffer with new pattern
  led_buffer.updateBufferWithArray(Spiral_LED_Array);
  led_buffer.SetAndIncrementUpdateIndex();
}
