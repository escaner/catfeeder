#include "config.h"
#include "pgtime.h"


/********************/
/* Member constants */
/********************/

// Maximum number of days per month (assumming no leap year)
const uint8_t PgTime::_NUM_DAYS_PER_MONTH[] =
  { 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U };

// Static tags in the display
const char *const PgTime::_LINES[DISPLAY_ROWS] PROGMEM = { _LINE0, _LINE1 };
const char PgTime::_LINE0[DISPLAY_COLS+1] PROGMEM = "TIEMPO    :  :  ";
const char PgTime::_LINE1[DISPLAY_COLS+1] PROGMEM = "UTC     /  /    ";


/***********/
/* Methods */
/***********/

/*
 *   Constructor. Initializes class and widgets.
 *  Parameters:
 *  * pParent: parent Page where to return the focus on exit
 *  * Lcd: reference to the lcd display that is being used.
 */
PgTime::PgTime(Page *pParent, LiquidCrystal &Lcd):
  Page(Lcd),
  _State(StOk),
  _pParent(pParent),
  _Widgets{
    WgInt(Lcd, _TIME_HOUR_COL, _TIME_ROW, _TIME_HOUR_SIZE),
    WgInt(Lcd, _TIME_MINUTE_COL, _TIME_ROW, _TIME_MINUTE_SIZE),
    WgInt(Lcd, _TIME_SECOND_COL, _TIME_ROW, _TIME_SECOND_SIZE),
    WgInt(Lcd, _DATE_DAY_COL, _DATE_ROW, _DATE_DAY_SIZE),
    WgInt(Lcd, _DATE_MONTH_COL, _DATE_ROW, _DATE_MONTH_SIZE),
    WgInt(Lcd, _DATE_YEAR_COL, _DATE_ROW, _DATE_YEAR_SIZE)
  }
{
}


/*
 *   Starts a series of actions and events to draw this page in the display.
 *  It first requests the UTC time. Uses _State to track the sequence of events.
 *  Returns:
 *  * PageAction requesting the current UTC time or no action when
 *    the process was already unchained and not finished.
 */
PageAction PgTime::focus()
{
  // Only makes sense at StOk
  if (_State == StOk)
  {
    // Request time
    _State = StNeedUtc;
    return PageAction(Action::AcNeedTimeUtc);
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
PageAction PgTime::event(const Event &E)
{
#pragma GCC diagnostic push
// Disable: warning: enumeration value not handled in switch
#pragma GCC diagnostic ignored "-Wswitch"

  switch (E.Id)
  {
  case Event::EvInit:
    // Initialize: display page for the first time
    return focus();

  case Event::EvSwitch:  // Switch has been pressed / released / something.
    // Pass event to current widget with the focus
    switch (_Widgets[_FocusWidget].event(E))
    {
    case Widget::AcBack:
      // Check correctness of date
      if (_validDate())
      {
        // Date is correct -> return it...
        Action A(Action::AcSetTimeUtc);
        A.Time = DateTime(_Values[WgYear], _Values[WgMonth], _Values[WgDay],
          _Values[WgHour], _Values[WgMinute], _Values[WgSecond]);
        // ... and go back to parent page
        return PageAction(_pParent, A);
      }
      else
        // Invalid date -> focus the day widget and perform no action
        _focusWidget(WgDay);
      break;
    case Widget::AcOk:
      // Move focus to next widget and return no action
      _focusNextWidget();
      break;
    // case Widget::AcNone: -> no action
    }
    break;

  case Event::EvTimeUtc:
    // Check if we requested it from focus()
    if (_State == StNeedUtc)
    {
      // Yes, all data received, we can finish initialization
      _State = StOk;
      // Initialize page and widgets
      _init(E.Time);
    }
    // We don't need any more data -> return no action
    break;
  }

#pragma GCC diagnostic pop

  // Default: no action
  return PageAction();
}


/*
 *   Initializes page and widgets.
 *  Parameters:
 *  * Time: date and time in UTC to initialize values.
 */
void PgTime::_init(const DateTime &Time)
{
  // Initialize values
  _Values[WgHour] = Time.hour();
  _Values[WgMinute] = Time.minute();
  _Values[WgSecond] = Time.second();
  _Values[WgDay] = Time.day();
  _Values[WgMonth] = Time.month();
  _Values[WgYear] = Time.year();

   // Draw page
  _Lcd.setCursor(0, _TIME_ROW);
  _Lcd.print((const __FlashStringHelper *) pgm_read_ptr(_LINES + _TIME_ROW));
  _Lcd.setCursor(0, _DATE_ROW);
  _Lcd.print((const __FlashStringHelper *) pgm_read_ptr(_LINES + _DATE_ROW));

  // Initialize & draw widgets
  _Widgets[WgHour].init(_MIN_HOUR, _MAX_HOUR, _Values+WgHour);
  _Widgets[WgMinute].init(_MIN_MINUTE, _MAX_MINUTE, _Values+WgMinute);
  _Widgets[WgSecond].init(_MIN_SECOND, _MAX_SECOND, _Values+WgSecond);
  _Widgets[WgDay].init(_MIN_DAY, _MAX_DAY, _Values+WgDay);
  _Widgets[WgMonth].init(_MIN_MONTH, _MAX_MONTH, _Values+WgMonth);
  _Widgets[WgYear].init(_MIN_YEAR, _MAX_YEAR, _Values+WgYear);

  // Set focus on hour
  _focusWidget(WgHour);
}


/*
 *   Moves focus to the next widget.
 */
void PgTime::_focusNextWidget()
{
  // Calculate next widget call method to give him the focus
  _focusWidget(WgId_t((uint8_t(_FocusWidget) + uint8_t(1U)) % _NUM_WIDGETS));
}


/*
 *   Moves focus to the widget.
 *  Parameters:
 *  * WgId: wich widget should be getting the focus.
 */
void PgTime::_focusWidget(WgId_t WgId)
{
  // Update focus widget
  _FocusWidget = WgId;

  // Notify new widget that he has the focus
  _Widgets[_FocusWidget].focus();
}


/*
 *   Checks whether the day is valid within the date
 *  Returns:
 *  * true: the date is valid
 *  * false: the date is invalid
 */
bool PgTime::_validDate() const
{
  uint8_t Month = _Values[WgMonth];
  uint8_t MaxDay = _NUM_DAYS_PER_MONTH[Month];

  // Special case for February on leap year
  if (Month == 2 && _leapYear(_Values[WgYear]))
    MaxDay++;

  // Check correctness of the day
  return _Values[WgDay] <= MaxDay;
}


/*
 *   Checks whether the Year is a leap year.
 *  Parameters:
 *  * Year: the year to check.
 *  Returns:
 *  * true: Year is a leap year
 *  * false: Year is not a leap year
 */
bool PgTime::_leapYear(uint16_t Year) const
{
  return (Year%4==0 && Year%100!=0) || Year%400==0;
}
