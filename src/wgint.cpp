#include <cassert>
#include "wgint.h"


/*
 *   Constructor. Initializes the object. Note that _MinValue and _MaxValue
 *  are not initialized. They can change from call to call.
 */
WgInt::WgInt(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows,
    uint8_t PosX, uint8_t PosY, uint8_t Size):
  Widget(Lcd, Cols, Rows),
  _X(PosX),
  _Y(PosY),
  _Size(Size)
{
}

/*
 *   Sets the initial value and valid range of values.
 *  Paramters:
 *  * MinValue: minimum possible value for the widget.
 *  * MaxValue: maximum possible value for the widget.
 *  * InitValue: initial value for the widget.
 */
void WgInt::init(uint16_t MinValue, uint16_t MaxValue, uint16_t InitValue)
{
  // Check that initial value is within range
  assert(InitValue >= MinValue && InitValue <= MaxValue);

  // Check that values fit in the _Size of the widget (negative need also sign)
  assert(MinValue == 0U ||
    (uint8_t(log10(abs(_MinValue))) + (MinValue<0U? uint8_t(2U): uint8_t(1U)))
    <= _Size);
  assert(MaxValue == 0U ||
    (uint8_t(log10(abs(_MaxValue))) + (MaxValue<0U? uint8_t(2U): uint8_t(1U)))
    <= _Size);

  // Initialize data
  _MinValue = MinValue;
  _MaxValue = MaxValue;
  _Value = InitValue;
}


/*
 *   Display value on the LCD and start blinking.
 */
void WgInt::focus()
{
  _draw();
  _blinkOn();
}

/*
 *   Process switches events.
 *  Parameters:
 *  * E: event
 *  Returns:
 *  * >= 0: selected value
 *  * EvNone: no further action required
 *  * EvBack: go back to the previous page
 */
int16_t WgInt::event(const Event &E)
{
  int16_t Ac = AcNone;  // Default action initialized

  // Only process switch events, ignore the rest
  if (E.Id == Event::EvSwitch)
    switch (E.Switch)
    {
    case Event::SwEvSelectCw:
      // Decrease value option
      _decrement();
      // Ac = AcNone;
      break;
    case Event::SwEvSelectCcw:
      // Increase value option
      _increment();
      // Ac = AcNone;
      break;
    case Event::SwEvEnterPress:
      // Return currently selected value and disble blinking
      _blinkOff();
      Ac = _Value;
      break;
    case Event::SwEvBackPress:
      // Signal to go back and disble blinking
      _blinkOff();
      Ac = AcBack;
      break;
    }

  return Ac;
}


/*
 *   Displays current _Value on the LCD.
 */
void WgInt::_draw() const
{
  char szValue[_Size + 1];

  // Move cursor to widget place
  _Lcd.setCursor(_X, _Y);

  // Convert value to text. Is should fit.
  sprintf(szValue, "%0*d", _Size, _Value);
  _Lcd.print(szValue);
}


/*
 *   Clears current _Value on the LCD for the blinking effect.
 */
void WgInt::_clear() const
{
  uint8_t Size = _Size;

  // Move cursor to widget place
  _Lcd.setCursor(_X, _Y);

  // Overwrite with blanks
  while (Size--)
    _Lcd.write(' ');
}


/*
 *   Enable Interrupt Service Routine to manage the blinking.
 */
void WgInt::_blinkOn()
{
  // We are displaying the value
  _BlinkClear = false;

  // Enable ISR
}


/*
 *   Disable the Interrupt Service Routine managing the blinking.
 */
void WgInt::_blinkOff()
{
  // Disable ISR
}


/*
 *   Callback to make the LCD value blink.
 */
void WgInt::_isrBlink()
{
  if (_BlinkClear)
  {
    _draw();
    _BlinkClear = false;
  }
  else
  {
    _clear();
    _BlinkClear = true;
  }
}


/*
 *   Increments current value within limits.
 */
void WgInt::_increment()
{
  if (_Value != _MaxValue)
    _Value++;
}


/*
 *   Decrements current value within limits.
 */
void WgInt::_decrement()
{
  if (_Value != _MinValue)
    _Value--;
}
