#define __ASSERT_USE_STDERR
#include <cassert>
#include "wgint.h"
#include "timer1.h"


/***************/
/* Friend stuff */
/***************/

// Pointer to object used by _isrWgIntBlink to make it blink
static WgInt *pBlinkObj;

/*
 *   Interrupt Service Routine to make the LCD value blink. Defined out of the
 *  class to match the void (*)() type. It will behave as belonging to object
 *  pBlinkObj.
 */
void _isrWgIntBlink()
{
  if (pBlinkObj->_BlinkClear)
  {
    pBlinkObj->_draw();
    pBlinkObj->_BlinkClear = false;
  }
  else
  {
    pBlinkObj->_clear();
    pBlinkObj->_BlinkClear = true;
  }
}


/***************/
/* Class stuff */
/***************/

/*
 *   Constructor. Initializes the object. Note that _MinValue and _MaxValue
 *  are not initialized. They can change from call to call.
 *  Parameters:
 *  * Lcd: reference to the lcd display that is being used.
 *  * Cols: number of columns in the LCD.
 *  * Rows: number of rows in the LCD.
 *  * PosX: column where to start displaying the widget.
 *  * PosY: row where to start displaying the widget.
 *  * Size: size of the arrays and of the widget (number of bools)
 */
WgInt::WgInt(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows,
    uint8_t PosX, uint8_t PosY, uint8_t Size):
  Widget(Lcd, Cols, Rows),
  _X(PosX),
  _Y(PosY),
  _Size(Size)
{
  assert(Size > 0U);
}


/*
 *   Sets the initial value and valid range of values.
 *  Parameters:
 *  * MinValue: minimum possible value for the widget.
 *  * MaxValue: maximum possible value for the widget.
 *  * InitValue: initial value for the widget.
 */
void WgInt::init(uint16_t MinValue, uint16_t MaxValue, uint16_t *pValue)
{
  // Check that initial value is within range
  assert(*pValue >= MinValue && *pValue <= MaxValue);

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
  *_pValue = *pValue;
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
 *  * EvOk: value accepted
 *  * EvNone: no further action required
 *  * EvBack: go back to the previous page
 */
int8_t WgInt::event(const Event &E)
{
  int8_t Ac = AcNone;  // Default action initialized

  // Only process switch events, ignore the rest
  if (E.Id == Event::EvSwitch)
    switch (E.Switch)
    {
    case Event::SwEvSelectCw:
      // Decrease value option and update LCD while blinking
      _decrement();
      _drawBlinking();
      // Ac = AcNone;
      break;
    case Event::SwEvSelectCcw:
      // Increase value option and update LCD while blinking
      _increment();
      _drawBlinking();
      // Ac = AcNone;
      break;
    case Event::SwEvEnterPress:
      // Return OK. Currently selected *pValue is accepted and disble blinking
      _blinkOff();
      Ac = AcOk;
      break;
    case Event::SwEvBackPress:
      // Signal to go back and disable blinking. *pValue is accepted.
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
  sprintf(szValue, "%0*d", _Size, *_pValue);
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
 *   Draws new value in the LCD while binking.
 */
void WgInt::_drawBlinking() const
{
  // MUTEX for the interrupt protecting _BlinkClear
  noInterrupts();

  // Draw the new value only while displaying a value, not on clear
  if (!_BlinkClear)
    _draw();

  // Exit the MUTEX
      interrupts();
}


/*
 *   Enable Interrupt Service Routine to manage the blinking.
 */
void WgInt::_blinkOn()
{
  // We are currently displaying the value
  _BlinkClear = false;

  // Enable ISR to blinking function and reference it to this obj
  pBlinkObj = this;
  enableIsr(_isrWgIntBlink);
}


/*
 *   Disable the Interrupt Service Routine managing the blinking.
 */
void WgInt::_blinkOff() const
{
  // Disable ISR for blinking
  disableIsr();
}


/*
 *   Increments current value within limits.
 */
void WgInt::_increment()
{
  if (*_pValue != _MaxValue)
    (*_pValue)++;
}


/*
 *   Decrements current value within limits.
 */
void WgInt::_decrement()
{
  if (*_pValue != _MinValue)
    (*_pValue)--;
}
