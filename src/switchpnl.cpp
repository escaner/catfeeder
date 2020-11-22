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
 *   Configures Arduino pins for switches, pulling them HIGH as required.
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
 *  detected.
 */
SwitchPnl::Event SwitchPnl::check()
{
  uint8_t Val0, Val1;
  int8_t StepSelect;
  Event Ret;

  // Check Select encoder
  Val0 = digitalRead(_PinSelectA);
  Val1 = digitalRead(_PinSelectB);
  if (StepSelect = _Select.update(Val0, Val1))
    return StepSelect < 0? EvSelectCcw: EvSelectCw;

  // Check Enter button: falling flank is button press
  Val0 = digitalRead(_PinEnter);
  if (_Enter.updateFlank(Val0) == Switch::FLANK_FALLING)
    return EvEnter;

  // Check Back button: falling flank is button press
  Val0 = digitalRead(_PinBack);
  if (_Back.updateFlank(Val0) == Switch::FLANK_FALLING)
    return EvBack;
}