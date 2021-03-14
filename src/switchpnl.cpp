#include "config.h"
#include <Arduino.h>
#include "switchpnl.h"

/*
 *   Constructor: initializes member data.
 *  The button values are pulled HIGH therefore that will be considered their
 *  initial state, being LOW when pressed.
 *  Parameters:
 *  * PinSelectA: Arduino pin where the A terminal of the encoder is connected.
 *  * PinSelectB: Arduino pin where the B terminal of the encoder is connected.
 *  * PinEnter:  Arduino pin where the Enter button is connected.
 *  * PinBack:  Arduino pin where the Back button is connected.
 */
SwitchPnl::SwitchPnl(uint8_t PinSelectA, uint8_t PinSelectB, uint8_t PinEnter,
    uint8_t PinBack):
  _PinSelectA(PinSelectA),
  _PinSelectB(PinSelectB),
  _PinEnter(PinEnter),
  _PinBack(PinBack),
  _Select(),
  _Enter(HIGH),  // Pulled HIGH for initial state
  _Back(HIGH)    // Pulled HIGH for initial state
{
}


/*
 *   Configures Arduino pins for switches, pulling them up as required.
 */
void SwitchPnl::init() const
{
  pinMode(_PinSelectA, INPUT_PULLUP);
  pinMode(_PinSelectB, INPUT_PULLUP);
  pinMode(_PinEnter, INPUT_PULLUP);
  pinMode(_PinBack, INPUT_PULLUP);
}


/*
 *   Reads status of switches pins and returns an Event when changes are
 *  detected. As it is needed for the switch readings to stabilize, one single
 *  read with a change will never deliver a switch change event. Therefore
 *  the LoopCnt parameter is provided to try a maximum of LoopCnt iterations
 *  before giving up and returning a no event status. The moment a switch change
 *  is positively registered, the method just returns it.
 *  Parameters:
 *  * LoopCnt: loop a maximum of LoopCnt iterations waiting for the switch
 *    to stabilize. If 0, no reads are taken and just returns SwEvNone.
 *  Returns: the registered switch event if any, otherwise SwEvNone.
 */
Event::SwitchEvent SwitchPnl::check(uint16_t LoopCnt)
{
  uint8_t Val0, Val1;
  int8_t StepSelect;

  // Iterate LoopCnt times waiting for the switch event to stabilize
  while (LoopCnt--)
  {
    // Check Select encoder
    Val0 = digitalRead(_PinSelectA);
    Val1 = digitalRead(_PinSelectB);
    StepSelect = _Select.update(Val0, Val1);
    if (StepSelect < 0)
      return Event::SwEvSelectCcw;
    else if (StepSelect > 0)
      return Event::SwEvSelectCw;

#pragma GCC diagnostic push
  // Disable: warning: enumeration value 'FLANK_NONE' not handled in switch
#pragma GCC diagnostic ignored "-Wswitch"

    // Check Enter button: falling flank is button press
    Val0 = digitalRead(_PinEnter);
    switch (_Enter.updateFlank(Val0))
    {
    case Switch::FLANK_FALLING:
      return Event::SwEvEnterPress;
    case Switch::FLANK_RISING:
      return Event::SwEvEnterRelease;
    }

    // Check Back button: falling flank is button press
    Val0 = digitalRead(_PinBack);
    switch (_Back.updateFlank(Val0))
    {
    case Switch::FLANK_FALLING:
      return Event::SwEvBackPress;
    case Switch::FLANK_RISING:
      return Event::SwEvBackRelease;
    }
  }

#pragma GCC diagnostic pop
 
  return Event::SwEvNone;
}