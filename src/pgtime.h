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
  PgTime(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows);
  virtual PageAction focus();
  virtual PageAction event(const Event &E);

protected:
  // Static constants

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
  static const uint8_t _MAX_DAY[_MAX_MONTH+1];
  static const uint8_t _MIN_YEAR = 2000U;
  static const uint8_t _MAX_YEAR = _MIN_YEAR + UINT8_MAX;

  // Positions of widgets and tags
  static const uint8_t _TIME_ROW = 0U;
  static const uint8_t _TIME_TAG_COL = 0U;
  static const uint8_t _TIME_HOUR_COL = 8U;
  static const uint8_t _TIME_MINUTE_COL = 11U;
  static const uint8_t _TIME_SECOND_COL = 14U;
  static const uint8_t _DATE_ROW = 1U;
  static const uint8_t _DATE_TAG_COL = 0U;
  static const uint8_t _DATE_DAY_COL = 6U;
  static const uint8_t _DATE_MONTH_COL = 9U;
  static const uint8_t _DATE_YEAR_COL = 12U;

  // Static tags in the display
  static const char PgTime::_TIME_TAG[];
  static const char PgTime::_DATE_TAG[];

  // Protected methods
  uint8_t _maxDay() const;

  // Member data

  // Widgets
  WgInt _WgHour;
  WgInt _WgMinute;
  WgInt _WgSecond;
  WgInt _WgDay;
  WgInt _WgMonth;
  WgInt _WgYear;

  // Values for widgets
  uint8_t _Hour;
  uint8_t _Minute;
  uint8_t _Second;
  uint8_t _Day;
  uint8_t _Month;
  uint16_t _Year;
};

#endif  // _PGTIME_H_
