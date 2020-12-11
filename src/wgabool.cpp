#include "config.h"
#include <assert.h>
#include "wgabool.h"
#include "timer1.h"


/***************/
/* Friend stuff */
/***************/

// Pointer to object used by _isrWgIntBlink to make it blink
static WgAbool *pBlinkObj;

/*
 *   Interrupt Service Routine to make the LCD value blink. Defined out of the
 *  class to match the void (*)() type. It will behave as belonging to object
 *  pBlinkObj.
 */
void _isrWgAboolBlink()
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
 *   Constructor. Initializes the object. Note that _pValues is not initialized.
 *  It can change from call to call.
 *  Parameters:
 *  * Lcd: reference to the lcd display that is being used.
 *  * PosX: column where to start displaying the widget.
 *  * PosY: row where to start displaying the widget.
 *  * pCharTrue: array with char to display when the value is true
 *  * pCharFalse: array with char to display when the value is false
 *  * Size: size of the arrays and of the widget (number of bools)
 */
WgAbool::WgAbool(LiquidCrystal &Lcd, uint8_t PosX, uint8_t PosY,
    const char *pCharTrue, const char *pCharFalse, uint8_t Size):
  Widget(Lcd),
  _X(PosX),
  _Y(PosY),
  _Size(Size),
  _pCharTrue(pCharTrue),
  _pCharFalse(pCharFalse)
{
  assert(Size > 0U);
}


/*
 *   Sets the initial values. It will also be used for output.
 *  Parameters:
 *  * InitValue: initial value for the widget.
 */
void WgAbool::init(bool *pValues)
{
  // Save in/out array for the values
  _pValues = pValues;

  // Display all the corresponding values in the widget
  _drawAll();
}


/*
 *   Focus to this widget: start blinking first position in the array.
 */
void WgAbool::focus()
{
  // Select initial position in the array
  _CurPos = 0U;

  // Start blinking
  _blinkOn();
}


/*
 *   Process switches events.
 *  Parameters:
 *  * E: event
 *  Returns:
 *  * EvOk: end; values updated in the array from init()
 *  * EvNone: no further action required
 *  * EvBack: go back to the previous page; values updated in the array
 */
int8_t WgAbool::event(const Event &E)
{
  int8_t Ac = AcNone;  // Default action initialized

#pragma GCC diagnostic push
// Disable: warning: enumeration value not handled in switch
#pragma GCC diagnostic ignored "-Wswitch"

  // Only process switch events, ignore the rest
  if (E.Id == Event::EvSwitch)
    switch (E.Switch)
    {
    case Event::SwEvSelectCw:
    case Event::SwEvSelectCcw:
      // Invert value and update LCD while blinking
      _pValues[_CurPos] = !_pValues[_CurPos];
      _drawBlinking();
      // Ac = AcNone;
      break;
    case Event::SwEvEnterPress:
      // Go to next entry in the array.
      if (_CurPos == _Size-1)
      {
        // No more entries: return currently selected values and disble blinking
        _blinkOff();
        Ac = AcOk;
      }
      else
      {
        _nextPos();
        // Ac = AcNone;
      }
      break;
    case Event::SwEvBackPress:
      // Signal to exit; disble blinking
      _blinkOff();
      Ac = AcBack;
      break;
    }

#pragma GCC diagnostic pop

  return Ac;
}


/*
 *   Draws all positions in the LCD according to their value in the
 *  array.
 */
void WgAbool::_drawAll() const
{
  uint8_t Pos;

  // Go to the position of the widget
  _Lcd.setCursor(_X, _Y);

  // Traverse every position in the values
  for (Pos = 0U; Pos<_Size; Pos++)
    // Write value at Pos position according to its value the Char arrays
    _Lcd.write(_getChar(Pos));
}


/*
 *   Draws corresponding char to the value ar _CurPos position.
 */
void WgAbool::_draw() const
{
  // Move LCD cursor to the position of the value
  _Lcd.setCursor(_X + _CurPos, _Y);

  // Write value at _CurPos position according to its value the Char arrays
  _Lcd.write(_getChar(_CurPos));
}


/*
 *   Clears the position of the value at _CurPos on the LCD, for blinking.
 */
void WgAbool::_clear() const
{
  // Move LCD cursor to the position of the value
  _Lcd.setCursor(_X + _CurPos, _Y);

  // Write blank
  _Lcd.write(' ');
}


/*
 *   While blinking, check if it is in LCD draw period and refresh it.
 */
void WgAbool::_drawBlinking() const
{
  // Mutex to make _BlinkClear access and _draw() atomic
  noInterrupts();

  // Are we in draw period?
  if (!_BlinkClear)
    // Yes -> draw again
    _draw();

  // End mutex
  interrupts();
}


/*
 *   Once checked that we are still in bounds, increase _CurPos and
 *  refresh drawing/clearing of the LCD.
 */
void WgAbool::_nextPos()
{
  // Mutex to correcly draw old and new position accessing _BlinkClear
  noInterrupts();

  // Are we in clear time?
  if (_BlinkClear)
  {
    // Yes -> leave old _CurPos drawn
    _draw();
    // We should have checked before that we are in bounds
    _CurPos++;
    // Clear new _CurPos
    _clear();
  }
  else
    // We are in drawn period, no need to draw or clear anything
    _CurPos++;

  // End mutex
  interrupts();
}


/*
 *   Enable Interrupt Service Routine to manage the blinking.
 */
void WgAbool::_blinkOn()
{
  // We are currently displaying the value
  _BlinkClear = false;

  // Enable ISR to blinking function and reference it to this obj
  pBlinkObj = this;
  enableIsr(_isrWgAboolBlink);
}


/*
 *   Disable the Interrupt Service Routine managing the blinking.
 */
void WgAbool::_blinkOff() const
{
  // Disable ISR for blinking
  disableIsr();

  // If it left in clear state, draw it
  if (_BlinkClear)
    _draw();
}
