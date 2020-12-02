#define __ASSERT_USE_STDERR
#include <cassert>
#include "pgmain.h"
#include "dotwtext.h"


/********************/
/* Member constants */
/********************/

// Text to display in the page
const char PgMain::_LINES[] =
{
  "%02hhu:%02hhu %c %02hhu/%02hhu/%02hhu",
  "SGTE %c%02hhu:%02hhu %s"
};

// Skip condition text. 0 -> no skip; 1 -> skip
const char PgMain::_SKIP_TEXT[] =
{
  "     ",
  "SALTA"
};


/***********/
/* Methods */
/***********/
/*
 *   Constructor.
 *  Paramters:
 *  * Lcd: reference to the lcd display that is being used.
 *  * Cols: number of columns in the LCD.
 *  * Rows: number of rows in the LCD.
 */
PgMain::PgMain(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows):
  Page(Lcd, Cols, Rows),
  _PgConfig(this, Lcd, Cols, Rows),
  _State(StOk),
  _ManFeeding(false)
{
}


/*
 *   Starts a series of actions and events to draw this page in the display.
 *  It firs requests the time, and then the next meal time. Uses _State
 *  to track the sequence of events.
 *  Returns:
 *  * PageAction requesting the current official time or no action when
 *    the process was already unchained and not finished.
 */
PageAction PgMain::focus()
{
  // Only makes sense at StOk
  if (_State == StOk)
  {
    // Request time
    _State = StNeedTime;
    return PageAction(Action::AcNeedTime);
  }

  return PageAction();
}


/*
 *   Manage events received my main page.
 *  Parameters:
 *  * E: event data
 *  Returns:
 *  * PageAction with instructions on how to proceed.
 */
PageAction PgMain::event(const Event &E)
{
  switch (E.Id)
  {
  case Event::EvInit:
    // Initialize: display page for the first time
    return focus();

  case Event::EvSwitch:  // Switch has been pressed / released / something.
    // Are we manually feeding?
    if (_ManFeeding)
      // Discard all switch events except for this one
      if (E.Switch == Event::SwEvBackRelease)
      {
        // End manual feeding
        _ManFeeding = false;
        return PageAction(Action::AcManualFeedEnd);
      }
      else
        // On any other switch, just continue feeding
        return PageAction(Action::AcManualFeedContinue);

    // else (no manually feeding), check other switches
    switch (E.Switch)
    {
    case Event::SwEvBackPress:
      // Start manual feeding
      _ManFeeding = true;
      return PageAction(Action::AcManualFeedStart);

    case Event::SwEvEnterPress:
      // Go to config page
      return PageAction(&_PgConfig);
    }
    // Default action for rest of switches
    break;

  case Event::EvTime:
    // Check if we requested it from focus()
    if (_State == StNeedTime)
      // Yes, advance state
      _State = StNeedNextMeal;
    // Either requested or because update, draw it
    _drawTime(E.Time);
    // Now request next meal data
    return PageAction(Action::AcNeedNextMeal);

  case Event::EvNextMeal:
    // Check if we requested it, unchained from focus()
    if (_State == StNeedNextMeal)
      // Yes, all data received
      _State = StOk;
    // Either requested or because update, draw it
    _drawNextMeal(E.NextMeal);
    // We don't need more data: no action
    break;
  }

  // Default: no action
  return PageAction();
}


/*
 *   Draws Time in the LCD.
 *  Parameters:
 *  * Time: time to be displayed.
 */
void PgMain::_drawTime(const DateTime &Time) const
{
  char Line[_COLS+1];  // Plus end of string
  char Dotw;
  uint8_t Year;

  // Move LCD cursor to time positon
  _Lcd.setCursor(_TIME_COL, _TIME_ROW);

  // Get single char representation of the day of the week
  Dotw = DotwText::DotwCharEs[Time.dayOfTheWeek()];
  // Get last 2 digits from the year
  Year = Time.year() % 100U;

  // Generate line to write
  sprintf(Line, _LINES[0],
    Time.hour(), Time.minute(), Dotw, Time.day(), Time.month(), Year);
  assert(strlen(Line) == _COLS);

  // Write line in LCD
  _Lcd.print(Line);
}


/*
 *   Draws Time in the LCD.
 *  Parameters:
 *  * Time: time to be displayed.
 */
void PgMain::_drawNextMeal(const Event::NextMeal_t &NextMeal) const
{
  char Line[_COLS+1];  // Plus end of string
  char Dotw;
  const char *pSkip;

  // Move LCD cursor to next meal position
  _Lcd.setCursor(_NEXTMEAL_COL, _NEXTMEAL_ROW);

  // Get single char representation of the day of the week
  Dotw = DotwText::DotwCharEs[NextMeal.Dotw];
  pSkip = _SKIP_TEXT[NextMeal.Skip? 1: 0];
  sprintf(Line, _LINES[1], NextMeal.Hour, NextMeal.Minute, Dotw, pSkip);
  assert(strlen(Line) == _COLS);

  // Write line in LCD
  _Lcd.print(Line);
}

