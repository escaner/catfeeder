#include "config.h"
#include "pgconfig.h"


/********************/
/* Member constants */
/********************/

// Text to display in the page
const char PgConfig::_LINE0[DISPLAY_COLS+1] PROGMEM = " SALTAR  COMIDAS";
const char PgConfig::_LINE1[DISPLAY_COLS+1] PROGMEM = " HORA    RESET  ";


/***********/
/* Methods */
/***********/

/*
 *   Constructor.
 *  Paramters:
 *  * pParent: parent Page where to return the focus on exit
 *  * Lcd: reference to the lcd display that is being used.
 */
PgConfig::PgConfig(Page *pParent, LiquidCrystal &Lcd):
  Page(Lcd),
  _pParent(pParent),
  _Select(Lcd, true),
  _PgMeal(this, Lcd),
  _PgTime(this, Lcd)
{
}


/*
 *   Draws the page and enables the select widget to select an option.
 *  Returns:
 *  * PageAction: with no action to perform.
 */
PageAction PgConfig::PgConfig::focus()
{
  // Draw page
  _Lcd.setCursor(0, 0);
  _Lcd.print((const __FlashStringHelper *) _LINE0);
  _Lcd.setCursor(0, 1);
  _Lcd.print((const __FlashStringHelper *) _LINE1);

  // Draw select widget
  _Select.focus();

  // Do nothing
  return PageAction();
}

/*
 *   Manage events received my config page. Is mostly passes the events to the
 *  Select widget, that manages the switches.
 *  Parameters:
 *  * E: event data
 *  Returns:
 *  * PageAction with instructions on how to proceed.
 */
PageAction PgConfig::event(const Event &E)
{
  // Pass event to Select widget
  switch (_Select.event(E))
  {
  case 0:
    // Skip/unskip next meal
    return PageAction(Action::AcSkipMeal);
  case 1:
    // Go to config meals page
    return PageAction(&_PgMeal);
  case 2:
    // Go to config time page
    return PageAction(&_PgTime);
  case 3:
    // Clear meals and reset board
    return PageAction(Action::AcReset);
  case Widget::AcBack:
    // Go back to parent page
    return PageAction(_pParent);
  }

  // Default action; do nothing
  return PageAction();
}
