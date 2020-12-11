#ifndef _WGINT_H_
#define _WGINT_H_

#include "config.h"
#include <Arduino.h>
#include "widget.h"


// ISR needs to be function of type void (*)() -> cannot be defined inside class
// because it would be defined as type void (*<class>::)(), so make it a global
// friend function
void _isrWgIntBlink();


/*
 *   Implements a widget to select an integer number.
 */
class WgInt: public Widget
{
public:
  WgInt(LiquidCrystal &Lcd, uint8_t PosX, uint8_t PosY, uint8_t Size);
  void init(uint16_t MinValue, uint16_t MaxValue, uint16_t *pValue);

  virtual void focus();
  virtual void unfocus();
  virtual int8_t event(const Event &E);

protected:
  friend void _isrWgIntBlink();

  // Protected methods
  void _draw() const;
  void _clear() const;
  void _drawBlinking() const;
  void _blinkOn();
  void _blinkOff() const;
  void _increment();
  void _decrement();

  // Member data
  const uint8_t _X;     // Column where to display the widget
  const uint8_t _Y;     // Row where to display the widget
  const uint8_t _Size;  // Number of charaters of the widget

  uint16_t *_pValue;   // Pointer to current value where it is kept updated
  uint16_t _MinValue;  // Maximum allowed value of _Value
  uint16_t _MaxValue;  // Minimum allowed value of _Value
  volatile bool _BlinkClear;  // When blinking: true iif displaying blank phase
};


#endif  // _WGINT_H_
