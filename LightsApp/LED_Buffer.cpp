#include "LED_Buffer.h"

LED_Buffer::LED_Buffer() {
    uint8_t i;
    for(i = 0; i < LED_BUFFER_SIZE; i++) {
      bufferIsUpdated[i] = false;
    }
    currIndexToBeUpdated = 0;
    currIndexToBeWritten = 0;
}

bool LED_Buffer::IsBufferReadyToBeWritten() {
  return bufferIsUpdated[currIndexToBeWritten];
}

void LED_Buffer::ClearAndIncrementWrittenIndex() {
  bufferIsUpdated[currIndexToBeWritten] = false;
  if(currIndexToBeWritten < (LED_BUFFER_SIZE - 1) ) {
    currIndexToBeWritten++;
  } else {
    currIndexToBeWritten = 0;
  }
}


bool LED_Buffer::IsBufferReadyToBeUpdated() {
  // If bufferIsUpdated is False then buffer is ready to be updated is True
  return !bufferIsUpdated[currIndexToBeUpdated];
}
void LED_Buffer::SetAndIncrementUpdateIndex() {
  bufferIsUpdated[currIndexToBeUpdated] = true;
  if(currIndexToBeUpdated < (LED_BUFFER_SIZE - 1) ) {
    currIndexToBeUpdated++;
  } else {
    currIndexToBeUpdated = 0;
  }
}

CRGB* LED_Buffer::getWriteArrayFromBuffer() {
  return bufferArray[currIndexToBeWritten];
}

void LED_Buffer::updateBufferWithArray(CRGB* newArray) {
  memcpy(bufferArray[currIndexToBeUpdated], newArray, NUM_LEDS*sizeof(CRGB));
}
