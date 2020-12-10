#include "config.h"
#include <assert.h>
#include "pgmain.h"
#include "dotwtext.h"


/********************/
/* Member constants */
/********************/

// Text to display in the page
const char PgMain::_LINE0[] PROGMEM = "%02u:%02u %c %02u/%02u/%02u";
const char PgMain::_LINE1[] PROGMEM = "SGTE %c%02u:%02u %s";
const char *const PgMain::_LINES[DISPLAY_ROWS] PROGMEM = { _LINE0, _LINE1 };

// Skip condition text. 0 -> normal, 1 -> served, 2 -> skip
const char PgMain::_STATUS_TEXT[][_NEXTMEAL_STATUS_SIZE+1U] =
{
  "    ",
  "SRVD",
  "SALT"
};


/***********/
/* Methods */
/***********/

/*
 *   Constructor.
 *  Paramters:
 *  * Lcd: reference to the lcd display that is being used.
 */
PgMain::PgMain(LiquidCrystal &Lcd):
  Page(Lcd),
  _State(StOk),
  _ManFeeding(false),
  _PgConfig(this, Lcd)
{
}


/*
 *   Starts a series of actions and events to draw this page in the display.
 *  It first requests the time, and then the next meal time. Uses _State
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
 *   Manage events received by page.
 *  Parameters:
 *  * E: event data
 *  Returns:
 *  * PageAction with instructions on how to proceed.
 */
PageAction PgMain::event(const Event &E)
{
#pragma GCC diagnostic push
// Disable: warning: enumeration value Ev* & SwEv* not handled in switch
#pragma GCC diagnostic ignored "-Wswitch"

  switch (E.Id)
  {
  case Event::EvInit:
    // Initialize: display page for the first time
    return focus();

  case Event::EvSwitch:  // Switch has been pressed / released / something.
    // Are we manually feeding?
    if (_ManFeeding)
    {
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
    }
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
    // Draw or update the time
    _drawTime(E.Time);

    // Check if we requested it from focus()
    if (_State == StNeedTime)
    {
      // Yes, advance state
      _State = StNeedNextMeal;
      // Now request next meal data
      return PageAction(Action::AcNeedNextMeal);
    }
    // Otherwise, just return no action
    break;

  case Event::EvNextMeal:
    // Check if we requested it, unchained from focus()
    if (_State == StNeedNextMeal)
    {
      // Yes, all data received, we can finish initialization
      _State = StOk;
    }
    // Either requested or because update, draw it
    _drawNextMeal(E.NextMeal);
    // We don't need more data: no action
    break;
  }

#pragma GCC diagnostic pop

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
  char Line[DISPLAY_COLS+1];  // Plus end of string
  char Dotw;
  uint8_t Year;

  // Move LCD cursor to time positon
  _Lcd.setCursor(_TIME_COL, _TIME_ROW);

  // Get single char representation of the day of the week
  Dotw = DotwText::DotwCharEs[Time.dayOfTheWeek()];
  // Get last 2 digits from the year
  Year = Time.year() % 100U;

  // Generate line to write
  sprintf_P(Line, pgm_read_ptr(_LINES + 0), (unsigned) Time.hour(),
    (unsigned) Time.minute(), Dotw, (unsigned) Time.day(),
    (unsigned) Time.month(), (unsigned) Year);
Serial.print(F("PgMain:Time:"));
Serial.println(Line);
Serial.print(F("strlen:"));
Serial.println(strlen(Line));
Serial.flush();
  assert(strlen(Line) == DISPLAY_COLS);

  // Write line in LCD
  _Lcd.write(Line);
}


/*
 *   Draws information about the next meal in the LCD.
 *  Parameters:
 *  * NextMeal: time, day of the week and status about the next meal.
 */
void PgMain::_drawNextMeal(const Event::NextMeal_t &NextMeal) const
{
  char Line[DISPLAY_COLS+1];  // Plus end of string
  char Dotw;
  const char *pStatus;

  // Move LCD cursor to next meal position
  _Lcd.setCursor(_NEXTMEAL_COL, _NEXTMEAL_ROW);

  // Is there a next meal?
  if (NextMeal.Status >= 0)
  {
    // Yes
    // Get single char representation of the day of the week
    Dotw = DotwText::DotwCharEs[NextMeal.Dotw];
    pStatus = _STATUS_TEXT[NextMeal.Status];
    sprintf_P(Line, pgm_read_ptr(_LINES + 1), Dotw, (unsigned) NextMeal.Hour,
      (unsigned) NextMeal.Minute, pStatus);
Serial.print(F("PgMain:NxtMeal:"));
Serial.println(Line);
Serial.flush();
  }
  else
  {
    // No next meal: fill with blanks
    memset(Line, ' ', DISPLAY_COLS * sizeof (char));
    // Add end of string
    Line[DISPLAY_COLS] = '\0';
  }
Serial.print(F("strlen:"));
Serial.println(strlen(Line));
Serial.flush();
  assert(strlen(Line) == DISPLAY_COLS);

  // Write line in LCD
  _Lcd.write(Line);
}

