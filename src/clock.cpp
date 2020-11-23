#include "clock.h"


/*************/
/* Constants */
/*************/

// Difference in seconds between standard and daylight saving times
const TimeSpan Clock::DST_DIFFERENCE(60*60);
const uint8_t Clock::DAYS_IN_A_WEEK = 7U;

// DST range
const uint8_t Clock::DST_START_MONTH = 3U;  // March
const uint8_t Clock::DST_START_MONTH_LAST_DAY = 31U;
const uint8_t Clock::DST_START_DOW = 0U;  // Sunday
const uint8_t Clock::DST_START_H = 0U;
const uint8_t Clock::DST_START_M = 0U;
const uint8_t Clock::DST_START_S = 0U;
const uint8_t Clock::DST_END_MONTH = 10U;  // October
const uint8_t Clock::DST_END_MONTH_LAST_DAY = 31U;
const uint8_t Clock::DST_END_DOW = 0U;  // Sunday
const uint8_t Clock::DST_END_H = 0U;
const uint8_t Clock::DST_END_M = 0U;
const uint8_t Clock::DST_END_S = 0U;


/***********/
/* Methods */
/***********/


/*
 *   Constructor.
 *  Parameters:
 *  * TzDiff: signed offset from UTC to local standard time (out of DST) in
 *    seconds.
 *  * EnableDst: whether to enable or not DST stuff for methods in this class.
 */
Clock::Clock(int32_t TzDiff, bool EnableDst):
  _Timezone(TzDiff),
  _DstEnabled(EnableDst)
{
}


/*
 *   Initializes RTC in the class and check that it runs properly.
 *  Returns: true iff an error was found.
 */
bool Clock::init() const
{
  return !_Rtc.begin() || !_Rtc.isrunning();
}


/*
 *   Sets time both for the RTC, in UTC.
 *  Parameters:
 *  * UtcTime: time and date in UTC format.
 */
void Clock::setUtc(const DateTime &UtcTime) const
{
  _Rtc.adjust(UtcTime);
}


/*
 *   Gets the current time from the RTC in UTC and returns it as is.
 */
DateTime Clock::getUtc() const
{
  return _Rtc.now();
}


/*
 *   Gets the current time from the RTC and returns it converted
 *  to Official time.
 */
DateTime Clock::getOfficial() const
{
  return _utcToOfficial(_Rtc.now());
}


/*
 *   Converts an UTC time into Official time (local time with DST applied
 *  as required).
 *  Parameters:
 *  * UtcTime: UTC time to convert
 *  Returns: the official time for UtcTime.
 */
DateTime Clock::_utcToOfficial(DateTime UtcTime) const
{
  DateTime OfficialTime;

  if (_inDst(UtcTime))
    OfficialTime = UtcTime + _Timezone + DST_DIFFERENCE;
  else
    OfficialTime = UtcTime + _Timezone;

  return OfficialTime;
}


/*
 *   Calculates whether a local time is within DST limits for Europe.
 *  DST starts: last Sunday March at 1:00 UTC
 *  DST ends: last Sunday October at 1:00 UTC
 *  Parameters:
 *  * UtcTime: UTC time to check if it is whithin DST limits.
 *  Returns: true iff Time is within DST limits.
 */
bool Clock::_inDst(const DateTime &UtcTime) const
{
  DateTime StartTime, EndTime;
  uint16_t Year;
  uint8_t DiffDays, StartDay, EndDay;
  bool Dst;

  if (_DstEnabled)
  {
    Year = UtcTime.year();
    // Make DST start: last Sunday March 01:00
    StartDay = _getLastDowOfMonth(Year, DST_START_MONTH,
      DST_START_MONTH_LAST_DAY, DST_START_DOW);
    StartTime = DateTime(Year, DST_START_MONTH, StartDay, DST_START_H,
      DST_START_M, DST_START_S);

    // Make DST end: last Sunday October 01:00
    EndDay = _getLastDowOfMonth(Year, DST_END_MONTH, DST_END_MONTH_LAST_DAY,
      DST_END_DOW);
    EndTime = DateTime(Year, DST_END_MONTH, EndDay, DST_END_H,
      DST_END_M, DST_END_S);

    Dst = UtcTime >= StartTime && UtcTime < EndTime;
  }
  else
    Dst = false;

  return Dst;
}


/*
 *   Calculates the day of the month for the last day of the week (Dow)
 *  appearance.
 *  Parameters:
 *  * Year
 *  * Month
 *  * LastDom: last day of the month in Month
 *  * Dow: day of the week that we need
 *  Returns: day of the month where matching the last Dow in Month
 */
uint8_t Clock::_getLastDowOfMonth(uint16_t Year, uint8_t Month,
  uint8_t LastDom, uint8_t Dow) const
{
  uint8_t LastDomDow;  // Day of the week for the last day in the month
  uint8_t DiffDays;

  // Make a date for the last day in the month and get the day of the week
  DateTime Date(Year, Month, LastDom);
  LastDomDow = Date.dayOfTheWeek();

  // Calculate the difference in days with the required day of the week
  if (LastDomDow < Dow)
    LastDomDow += DAYS_IN_A_WEEK;  // Avoid the overflow
  DiffDays = LastDomDow - Dow;

  // Get the actual day of the month
  return LastDom - DiffDays;
}
