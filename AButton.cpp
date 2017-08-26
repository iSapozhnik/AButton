#include "AButton.h"

AButton::AButton(int pin, int activeLow) {
  _pin = pin;
  _debounceTicks = 50;      // number of millisec that have to pass by before a click is assumed as safe.
  _clickTicks = 50;        // number of millisec that have to pass by before a click is detected.

  _state = 0; // starting with state 0: waiting for button to be pressed

  if (activeLow) {
    // the button connects the input pin to GND when pressed.
    _buttonReleased = HIGH; // notPressed
    _buttonPressed = LOW;

    // use the given pin as input and activate internal PULLUP resistor.
    pinMode( pin, INPUT_PULLUP );

  } else {
    // the button connects the input pin to VCC when pressed.
    _buttonReleased = LOW;
    _buttonPressed = HIGH;

    // use the given pin as input
    pinMode(pin, INPUT);
  }

  // no functions attached yet: clear all function pointers.
  _clickFunc = NULL;
}

// explicitly set the number of millisec that have to pass by before a click is assumed as safe.
void AButton::setDebounceTicks(int ticks) {
  _debounceTicks = ticks;
}

// explicitly set the number of millisec that have to pass by before a click is detected.
void AButton::setClickTicks(int ticks) {
  _clickTicks = ticks;
}

// save function for click event
void AButton::attachClick(callbackFunction newFunction) {
  _clickFunc = newFunction;
}

void AButton::tick(void)
{
  // Detect the input information
  int buttonLevel = digitalRead(_pin); // current button signal.
  unsigned long now = millis(); // current (relative) time in msecs.

  // Implementation of the state machine
  if (_state == 0) { // waiting for menu pin being pressed.
    if (buttonLevel == _buttonPressed) {
      _state = 1; // step to state 1
      _startTime = now; // remember starting time
    } // if

  } else if (_state == 1) { // waiting for menu pin being released.

    if ((buttonLevel == _buttonReleased) && ((unsigned long)(now - _startTime) < _debounceTicks)) {
      // button was released to quickly so I assume some debouncing.
	  // go back to state 0 without calling a function.
      _state = 0;

    } else if (buttonLevel == _buttonReleased) {
      _state = 2; // step to state 2
      _stopTime = now; // remember stopping time

    } else {
      // wait. Stay in this state.
    } // if

  } else if (_state == 2) { // waiting for menu pin being pressed the second time or timeout.
    if ((unsigned long)(now - _startTime) > _clickTicks) {
      // this was only a single short click
      if (_clickFunc) _clickFunc();
      _state = 0; // restart.

    } else if ((buttonLevel == _buttonPressed) && ((unsigned long)(now - _stopTime) > _debounceTicks)) {
      _state = 3; // step to state 3
      _startTime = now; // remember starting time
    } // if

  }
  // else if (_state == 3) { // waiting for menu pin being released finally.
  //   // Stay here for at least _debounceTicks because else we might end up in state 1 if the
  //   // button bounces for too long.
  //   if (buttonLevel == _buttonReleased && ((unsigned long)(now - _startTime) > _debounceTicks)) {
  //     // this was a 2 click sequence.
  //     if (_doubleClickFunc) _doubleClickFunc();
  //     _state = 0; // restart.
  //   } // if
  //
  // } else if (_state == 6) { // waiting for menu pin being release after long press.
  //   if (buttonLevel == _buttonReleased) {
	//   _isLongPressed = false;  // Keep track of long press state
	//   if(_longPressStopFunc) _longPressStopFunc();
  //     _state = 0; // restart.
  //   } else {
	//   // button is being long pressed
	//   _isLongPressed = true; // Keep track of long press state
	//   if (_duringLongPressFunc) _duringLongPressFunc();
  //   } // if
  //
  // }
}
