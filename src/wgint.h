#ifndef _WGINT_H_
#define _WGINT_H_

#include "widget.h"


/*
 *   Implements a widget to select an integer number.
 */
class WgInt: public Widget
{
public:
  WgInt(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows,
    uint8_t PosX, uint8_t PosY, uint8_t Size);
  void init(uint16_t MinValue, uint16_t MaxValue, uint16_t InitValue);

  virtual void focus();
  virtual int16_t event(const Event &E);

protected:
  static const _BLINK_TIME = ;  // Microseconds

  // Protected methods
  void _draw() const;
  void _clear() const;
  void _drawBlinking() const;
  void _blinkOn();
  void _blinkOff();
  void _isrBlink();
  void _increment();
  void _decrement();

  // Member data
  const uint8_t _X;     // Column where to display the widget
  const uint8_t _Y;     // Row where to display the widget
  const uint8_t _Size;  // Number of charaters of the widget

  uint16_t _Value;     // Current value
  uint16_t _MinValue;  // Maximum allowed value of _Value
  uint16_t _MaxValue;  // Minimum allowed value of _Value
  volatile bool _BlinkClear;  // When blinking: true iif displaying blank phase
};


#endif  // _WGINT_H_
