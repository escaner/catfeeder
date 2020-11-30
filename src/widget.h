#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <LiquidCrystal.h>
#include "event.h"


class Widget
{
public:
  // Standard values for actions on event() status return
  static const int16_t AcNone = INT16_MIN;
  static const int16_t AcBack = INT16_MIN+1;

  Widget(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows):
    _Lcd(Lcd),
    _COLS(Cols),
    _ROWS(Rows)
  {
  }

  // Abstract virtual methods
  virtual void focus() = 0;
  virtual int16_t event(const Event &E) = 0;

protected:
  // Member data
  const uint8_t _COLS;
  const uint8_t _ROWS;
  LiquidCrystal &_Lcd;
};

#endif  // _WIDGET_H_