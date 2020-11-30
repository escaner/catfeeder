#include "wgselect.h"


/********************/
/* Static constants */
/********************/

const uint8_t WgSelect::_Coord2Opt[2][2] = { {0, 0}, {0, 1} };
const uint8_t WgSelect::_Coord4Opt[4][2] = { {0, 0}, {8, 0}, {0, 1}, {8, 1} };


/***********/
/* Methods */
/***********/

/*
 *   Constructor. Initializes the object as a 2 or 4 select widget.
 */
WgSelect::WgSelect(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows,
    bool FourOptions):
  Widget(Lcd, Cols, Rows),
  _NumOptions(FourOptions? 4U: 2U),
  _CoordOpt(FourOptions? _Coord4Opt: _Coord2Opt)
{
}


/*
 *   The widget got focus. Initialize and draw the cursor at the default option.
 */
void WgSelect::focus()
{
  // Reset current option and draw the cursor
  _CurOption = 0U;
  _drawCursor();
}


/*
 *   Process switches events.
 *  Parameters:
 *  * E: event
 *  Returns:
 *  * >= 0: value with the option selected
 *  * EvNone: no further action required
 *  * EvBack: go back to the previous page
 */
int16_t WgSelect::event(const Event &E)
{
  int16_t Ac = AcNone;  // Default action initialized

  // Only process switch events, ignore the rest
  if (E.Id == Event::EvSwitch)
    switch (E.Switch)
    {
    case Event::SwEvSelectCw:
      // Select previous option
      _clearCursor();
      _prevOption();
      _drawCursor();
      // Ac = AcNone;
      break;
    case Event::SwEvSelectCcw:
      // Select next option
      _clearCursor();
      _nextOption();
      _drawCursor();
      // Ac = AcNone;
      break;
    case Event::SwEvEnterPress:
      // Return currently selected option
      Ac = (int16_t) _CurOption;
      break;
    case Event::SwEvBackPress:
      // Signal to go back
      Ac = AcBack;
      break;
    }

  return Ac;
}


/*
 *   Draws the cursor according to the currently selected option.
 */
void WgSelect::_drawCursor() const
{
  const uint8_t *pCoord = _CoordOpt[_CurOption];

  // Go to position and display the cursor
  _Lcd.setCursor(pCoord[0], pCoord[1]);
  _Lcd.write(_Cursor);
}


/*
 *   Deletes the cursor according to the currently selected option.
 */
void WgSelect::_clearCursor() const
{
  const uint8_t *pCoord = _CoordOpt[_CurOption];

  // Go to position and display the cursor
  _Lcd.setCursor(pCoord[0], pCoord[1]);
  _Lcd.write(' ');
}


/*
 *   Updates _CurOption with previous option in sequence.
 */
void WgSelect::_prevOption()
{
  // Watch out for underflow when option 0 selected
  if (!_CurOption)
    _CurOption += _NumOptions;

  // Calculate new option, no need for modulo anymore
  _CurOption--;
}


/*
 *   Updates _CurOption with next option in sequence.
 */
void WgSelect::_nextOption()
{
  _CurOption = (_CurOption + uint8_t(1U)) % _NumOptions;
}
