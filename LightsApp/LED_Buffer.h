#ifndef LED_BUFFER_H
#define LED_BUFFER_H

#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS 150
#define LED_BUFFER_SIZE 3
//
class LED_Buffer {
  private:
    bool bufferIsUpdated[LED_BUFFER_SIZE];   // Determines if buffer at index is updated
    uint8_t currIndexToBeUpdated;   // Index of the buffer being updated
    uint8_t currIndexToBeWritten;   // Index of the buffer to be written to LEDs    
    
  public:
    CRGB bufferArray[LED_BUFFER_SIZE][NUM_LEDS]; // Buffer of CRGB led states
    LED_Buffer();
    bool IsBufferReadyToBeWritten();
    void ClearAndIncrementWrittenIndex();
    bool IsBufferReadyToBeUpdated();
    void SetAndIncrementUpdateIndex();
    CRGB* getWriteArrayFromBuffer();
    void updateBufferWithArray(CRGB* newArray);
       
};


#endif
