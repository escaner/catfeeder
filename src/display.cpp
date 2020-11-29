#include <assert.h>
#include "display.h"

/*
 *   Constructor. Initializes LCD and window management structure, setting
 *  the focus to the main window.
 */
Display::Display(uint8_t PinRs, uint8_t PinEnable, uint8_t PinD4, uint8_t PinD5,
    uint8_t PinD6, uint8_t PinD7):
  _Lcd(PinRs, PinEnable, PinD4, PinD5, PinD6, PinD7),
  _PgMain(_Lcd, _COLS, _ROWS),
  _pFocusPage(&_PgMain)
{
  _Lcd.begin(_COLS, _ROWS);
}

/*
 *   Manages an event notification to the Display, passing it to the Page
 *  with focus.
 */
Action Display::event(const Event &E)
{
  PageAction PgA;
  Action A;

  // Pass event to focus page
  PgA = _pFocusPage->event(E);

  // Is there a focus change?
  if (PgA.pFocusPage != nullptr)
  {
    assert(PgA.pFocusPage != _pFocusPage);  // Focus really changes
    assert(PgA.Action.Id == Action::AcNone);  // No more actions on focus change

    // Update focus page and draw it
    _pFocusPage = PgA.pFocusPage;
    PgA = _pFocusPage->focus();
  }

  // Return the standard Action
  return PgA.Action;
}