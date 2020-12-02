#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <LiquidCrystal.h>
#include "event.h"


class Widget
{
public:
  // Standard values for actions on event() status return
  static const int8_t AcOk = -1;
  static const int8_t AcNone = -2;
  static const int8_t AcBack = -3;

  Widget(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows):
    _Lcd(Lcd),
    _COLS(Cols),
    _ROWS(Rows)
  {
  }

  // Abstract virtual methods
  virtual void focus() = 0;
  virtual int8_t event(const Event &E) = 0;

protected:
  // Member data
  const uint8_t _COLS;
  const uint8_t _ROWS;
  LiquidCrystal &_Lcd;
};

#endif  // _WIDGET_H_