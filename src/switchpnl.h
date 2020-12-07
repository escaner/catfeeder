#ifndef _SWITCHPNL_H_
#define _SWITCHPNL_H_

#include "config.h"
#include <Arduino.h>
#include <REncoder.h>
#include <Switch.h>
#include "event.h"


/*
 *   Class to manage switch panel inputs to the Arduino microcontroller in
 *  Catfeeder. The 
 */
class SwitchPnl
{
public:
  // Public methods
  SwitchPnl(uint8_t PinSelectA, uint8_t PinSelectB, uint8_t PinEnter,
    uint8_t PinBack);
  void init() const;
  Event::SwitchEvent check();

protected:
  uint8_t _PinSelectA, _PinSelectB;  // Pins used by rotary encoder Select
  uint8_t _PinEnter;  // Pushbutton Enter (in Select rotary encoder)
  uint8_t _PinBack;   // Pushbutton Back

  REncoder _Select;  // Manage Select encoder states and debounce
  Switch _Enter;     // Manage Enter button debounce
  Switch _Back;      // Manage Back button debounce
};


#endif  // _SWITCHPNL_H_