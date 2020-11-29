#include "pgconfig.h"


/********************/
/* Member constants */
/********************/

// Text to display in the page
const char PgConfig::_LINES[] =
{
  " SALTAR  NO SALT",
  " HORA    COMIDAS"
}


/***********/
/* Methods */
/***********/

/*
 *   Constructor.
 *  Paramters:
 *  * Lcd: reference to the lcd display that is being used.
 */
PgConfig::PgConfig(Page *pParent, LiquidCrystal &Lcd, uint8_t Cols,
    uint8_t Rows):
  Page(Lcd, Cols, Rows),
  _pParent(pParent),
  _Select(_NUM_OPTIONS)
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
  _Lcd.print(_LINES[0]);
  _Lcd.setCursor(0, 1);
  _Lcd.print(_LINES[1]);

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
    // Skip next meal
    return PageAction(Action::AcSkipMeal);
  case 1:
    // Unskip next meal
    return PageAction(Action::AcUnskipMeal);
  case 2:
    // Go to config meals page
    return PageAction(&_PgMeals);
  case 3:
    // Go to config time page
    return PageAction(&_PgTime);
  case Widget::EvBack:
    // Go back to parent page
    return PageAction(_pParent);
  }

  // Default action; do nothing
  return PageAction();
}
