#ifndef _PGTIME_H_
#define _PGTIME_H_

#include "page.h"
#include "wgint.h"


/*
 *   Class to create a page in the display to configure the current UTC time.
 */
class PgTime: public Page
{
public:
  PgTime(Page *pParent, LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows);
  virtual PageAction focus();
  virtual PageAction event(const Event &E);

protected:
  // Type for indexing the widgets and their values
  enum WgId_t: uint8_t
  {
    WgHour=0, WgMinute, WgSecond, WgDay, WgMonth, WgYear
  };

  // To keep track of the page initialization state
   enum State_t: uint8_t
  {
    StOk = 0U,
    StNeedUtc = 1U
  };

 // Static constants

  // How many widgets this page has
  static const uint8_t _NUM_WIDGETS = 6U;

  // Widget value limits
  static const uint8_t _MIN_HOUR = 0U;
  static const uint8_t _MAX_HOUR = 23U;
  static const uint8_t _MIN_MINUTE = 0U;
  static const uint8_t _MAX_MINUTE = 59U;
  static const uint8_t _MIN_SECOND = 0U;
  static const uint8_t _MAX_SECOND = 59U;
  static const uint8_t _MIN_MONTH = 1U;
  static const uint8_t _MAX_MONTH = 12U;
  static const uint8_t _MIN_DAY = 1U;
  static const uint8_t _MAX_DAY = 31U;
  static const uint8_t _MIN_YEAR = 2000U;
  static const uint8_t _MAX_YEAR = _MIN_YEAR + UINT8_MAX;

  // Positions of widgets and tags
  static const uint8_t _TIME_ROW = 0U;
  static const uint8_t _TIME_TAG_COL = 0U;
  static const uint8_t _TIME_HOUR_COL = 8U;
  static const uint8_t _TIME_HOUR_SIZE = 2U;
  static const uint8_t _TIME_MINUTE_COL = 11U;
  static const uint8_t _TIME_MINUTE_SIZE = 2U;
  static const uint8_t _TIME_SECOND_COL = 14U;
  static const uint8_t _TIME_SECOND_SIZE = 2U;
  static const uint8_t _DATE_ROW = 1U;
  static const uint8_t _DATE_TAG_COL = 0U;
  static const uint8_t _DATE_TAG_SIZE = 2U;
  static const uint8_t _DATE_DAY_COL = 6U;
  static const uint8_t _DATE_DAY_SIZE = 2U;
  static const uint8_t _DATE_MONTH_COL = 9U;
  static const uint8_t _DATE_MONTH_SIZE = 2U;
  static const uint8_t _DATE_YEAR_COL = 12U;
  static const uint8_t _DATE_YEAR_SIZE = 4U;
  static const uint8_t _NUM_DAYS_PER_MONTH[];

  // Static tags in the display
  static const char PgTime::_TIME_TAG[];
  static const char PgTime::_DATE_TAG[];

  // Protected methods
  void _init(const DateTime &Time);
  void _focusNextWidget();
  void _focusWidget(WgId_t WgId);
  bool _validDate() const;
  bool _leapYear(uint16_t Year) const;

  // Member data
  State_t _State;                  // Current initialization state
  Page *_pParent;                  // Parent page
  WgInt _Widgets[_NUM_WIDGETS];    // Widgets in this page
  uint16_t _Values[_NUM_WIDGETS];  // Values for the widgets
  WgId_t _FocusWidget;          // Wich widget has the focus
};

#endif  // _PGTIME_H_
