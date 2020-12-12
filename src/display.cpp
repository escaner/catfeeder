#include "config.h"
#include <assert.h>
#include "display.h"

/*
 *   Constructor. Initializes LCD and window management structure, setting
 *  the focus to the main window.
 */
Display::Display(uint8_t PinRs, uint8_t PinEnable, uint8_t PinD4, uint8_t PinD5,
    uint8_t PinD6, uint8_t PinD7):
  _Lcd(PinRs, PinEnable, PinD4, PinD5, PinD6, PinD7),
  _PgMain(_Lcd),
  _pFocusPage(&_PgMain)
{
  _Lcd.begin(DISPLAY_COLS, DISPLAY_ROWS);
}


/*
 *   Manages an event notification to the Display, passing it to the Page
 *  with focus.
 */
Action Display::event(const Event &E)
{
  PageAction PgA;

  // Pass event to focus page
  PgA = _pFocusPage->event(E);

  // Is there a focus change?
  if (PgA.pFocusPage != nullptr)
  {
    assert(PgA.pFocusPage != _pFocusPage);  // Focus really changes

    // Update focus page and draw it
    _pFocusPage = PgA.pFocusPage;
    PageAction PgAFocus = _pFocusPage->focus();

    // First action overrides focus action: copy only if no action
    if (PgA.MainAction.Id == Action::AcNone)
      PgA = PgAFocus;
  }

  // Return the standard Action
  return PgA.MainAction;
}


/*
 *  Shows a message in the display indicating that it is being reset.
 */
void Display::resetMessage()
{
  // Clear the display and show the message
  _Lcd.clear();
  _Lcd.print(F("REINICIALIZANDO"));
  // Prepare cursor for animation
  _Lcd.setCursor(0, 1);
}


/*
 *   Called repeatedly to show an animation during reset.
 */
void Display::resetAnimation()
{
  _Lcd.write('.');
}


/*
 *   Shows an error message in the display.
 *  Parameters:
 *  * pMsg: string with the error message.
 */
void Display::error(const char *pMsg)
{
  _error();
  _Lcd.write(pMsg);
}


/*
 *   Shows an error message in the display. PROGMEM friendly version.
 *  Parameters:
 *  * pMsg: string with the error message.
 */
void Display::error(const __FlashStringHelper *pMsg)
{
  _error();
  _Lcd.print(pMsg);
}


/*
 *   Displays Error in the first line of the display and prepares the second
 *  one for the message.
 */
void Display::_error()
{
  _Lcd.clear();
  _Lcd.print(F("ERROR:"));
  _Lcd.setCursor(0, 1);
}
