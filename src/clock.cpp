#include "config.h"
#include "clock.h"
#include "dotwtext.h"


/*************/
/* Constants */
/*************/

// Difference in seconds between standard and daylight saving times
const TimeSpan Clock::_DST_DIFFERENCE(60*60);


/***********/
/* Methods */
/***********/


/*
 *   Constructor.
 *  Parameters:
 *  * TzDiff: signed offset from UTC to local standard time (out of DST) in
 *    minutes.
 *  * EnableDst: whether to enable or not DST stuff for methods in this class.
 */
Clock::Clock(int32_t TzDiff, bool EnableDst):
  _Timezone(TzDiff * 60UL),
  _DstEnabled(EnableDst)
{
}


/*
 *   Initializes RTC in the class and check that it works properly.
 *  Returns: true iff an error was found.
 */
bool Clock::init()
{
  return !_Rtc.begin();
}


/*
 *   Returns whether the clock is ticking or it is stopped.
 *  Returns: true iff the clock is ticking.
 */
bool Clock::isrunning()
{
  return _Rtc.isrunning();
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
    OfficialTime = UtcTime + _Timezone + _DST_DIFFERENCE;
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
  uint8_t StartDay, EndDay;
  bool Dst;

  if (_DstEnabled)
  {
    Year = UtcTime.year();
    // Make DST start: last Sunday March 01:00
    StartDay = _getLastDowOfMonth(Year, _DST_START_MONTH,
      _DST_START_MONTH_LAST_DAY, _DST_START_DOW);
    StartTime = DateTime(Year, _DST_START_MONTH, StartDay, _DST_START_H,
      _DST_START_M, _DST_START_S);

    // Make DST end: last Sunday October 01:00
    EndDay = _getLastDowOfMonth(Year, _DST_END_MONTH, _DST_END_MONTH_LAST_DAY,
      _DST_END_DOW);
    EndTime = DateTime(Year, _DST_END_MONTH, EndDay, _DST_END_H,
      _DST_END_M, _DST_END_S);

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
    LastDomDow += DotwText::DAYS_IN_A_WEEK;  // Avoid the overflow
  DiffDays = LastDomDow - Dow;

  // Get the actual day of the month
  return LastDom - DiffDays;
}
