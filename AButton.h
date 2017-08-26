#ifndef AButton_h
#define AButton_h

#include "Arduino.h"

extern "C" {
  typedef void (*callbackFunction)(void);
}

class AButton
{
  public:
    AButton(int pin, int active);

    // set # millisec after safe click is assumed.
    void setDebounceTicks(int ticks);

    // set # millisec after single click is assumed.
    void setClickTicks(int ticks);

    // attach functions that will be called when button was pressed in the specified way.
    void attachClick(callbackFunction newFunction);

    // call this function every some milliseconds for handling button events.
    void tick(void);

  private:
    int _pin;        // hardware pin number.
    int _debounceTicks; // number of ticks for debounce times.
    int _clickTicks; // number of ticks that have to pass by before a click is detected
    int _state;

    int _buttonReleased;
    int _buttonPressed;

    // These variables will hold functions acting as event source.
    callbackFunction _clickFunc;

    unsigned long _startTime; // will be set in state 1
    unsigned long _stopTime; // will be set in state 2
};

#endif
