#ifndef _WGABOOL_H_
#define _WGABOOL_H_

#include "widget.h"


// ISR needs to be function of type void (*)() -> cannot be defined inside class
// because it would be defined as type void (*<class>::)(), so make it a global
// friend function
void _isrWgAboolBlink();


/*
 *   Implements a widget to select an boolean values in an array.
 */
class WgAbool: public Widget
{
public:
  WgAbool(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows, uint8_t PosX,
    uint8_t PosY, const char *pCharTrue, const char *pCharFalse, uint8_t Size);
  void init(bool *pValues);

  virtual void focus();
  virtual int8_t event(const Event &E);

protected:
  friend void _isrWgAboolBlink();

  // Protected methods
  void _drawAll() const;
  void _draw() const;
  void _clear() const;
  void _drawBlinking() const;
  void _nextPos();
  void _blinkOn();
  void _blinkOff() const;
  inline uint8_t _getChar(uint8_t Pos) const;

  // Member data
  const uint8_t _X;     // Column where to display the widget
  const uint8_t _Y;     // Row where to display the widget
  const uint8_t _Size;  // Number of charaters of the widget
  const char *_pCharTrue;
  const char *_pCharFalse;

  bool *_pValues;      // Array of bool with current values
  uint8_t _CurPos;     // Current position in the array
  volatile bool _BlinkClear;  // When blinking: true iif displaying blank phase
};


/*
 *   Returns the char corresponding to the value at Pos.
 *  Parameters:
 *  * Pos: position inside the array of the value we are requesting
 *  Returns: the char representation of the value at Pos.
 */
inline uint8_t WgAbool::_getChar(uint8_t Pos) const
{
  return _pValues[Pos]? _pCharTrue[Pos]: _pCharFalse[Pos];
}


#endif  // _WGABOOL_H_
