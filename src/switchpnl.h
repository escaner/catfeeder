#ifndef _SWITCHPNL_H_
#define _SWITCHPNL_H_

#include <REncoder.h>
#include <Switch.h>


/*
 *   Class to manage switch panel inputs to the Arduino microcontroller in
 *  Catfeeder. The 
 */
class SwitchPnl
{
public:
  // Event that can be returned regarding the switch user input
  enum Event: uint8_t
  {
    EvNone=0,        // No input event
    EvSelectCcw,     // Counterclockwise event on the Select encoder
    EvSelectCw,      // Clockwise event on the Select encoder
    EvEnterPress,    // Enter button press event (Select button pressed)
    EvEnterRelease,  // Enter button release event (Select button released)
    EvBackPress,     // Back button press event
    EvBackRelease    // Back button release event
   };

  // Public methods
  SwitchPnl(uint8_t PinSelectA, uint8_t PinSelectB, uint8_t PinEnter,
    uint8_t PinBack);
  void init() const;
  Event check();

protected:
  uint8_t _PinSelectA, _PinSelectB;  // Pins used by rotary encoder Select
  uint8_t _PinEnter;  // Pushbutton Enter (in Select rotary encoder)
  uint8_t _PinBack;   // Pushbutton Back

  REncoder _Select;  // Manage Select encoder states and debounce
  Switch _Enter;     // Manage Enter button debounce
  Switch _Back;      // Manage Back button debounce
};


#endif  // _SWITCHPNL_H_