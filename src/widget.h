#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "config.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "event.h"


class Widget
{
public:
  // Standard values for actions on event() status return
  static const int8_t AcOk = -1;
  static const int8_t AcNone = -2;
  static const int8_t AcBack = -3;

  Widget(LiquidCrystal &Lcd):
    _Lcd(Lcd)
  {
  }

  // Abstract virtual methods
  virtual void focus() = 0;
  virtual int8_t event(const Event &E) = 0;

protected:
  // Member data
  LiquidCrystal &_Lcd;
};

#endif  // _WIDGET_H_