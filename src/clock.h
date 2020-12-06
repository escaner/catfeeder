#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <Arduino.h>
#include <RTClib.h>


/*
 *   Class for time management. It manages the Real Time Clock DS1307 time and
 *  also adjusts for timezone and Daylight Saving Time for Europe.
 *  It keeps time in UTC.
 */
class Clock
{
public:
  // Public methods
  Clock(int32_t TzDiff, bool EnableDst);
  bool init();
  void setUtc(const DateTime &UtcTime) const;
  DateTime getUtc() const;
  DateTime getOfficial() const;

protected:
  // Constants
  static const TimeSpan _DST_DIFFERENCE;

  // DST range, UTC times
  static const uint8_t _DST_START_MONTH = 3U;  // March
  static const uint8_t _DST_START_MONTH_LAST_DAY = 31U;
  static const uint8_t _DST_START_DOW = 0U;  // Sunday
  static const uint8_t _DST_START_H = 1U;
  static const uint8_t _DST_START_M = 0U;
  static const uint8_t _DST_START_S = 0U;
  static const uint8_t _DST_END_MONTH = 10U;  // October
  static const uint8_t _DST_END_MONTH_LAST_DAY = 31U;
  static const uint8_t _DST_END_DOW = 0U;  // Sunday
  static const uint8_t _DST_END_H = 1U;
  static const uint8_t _DST_END_M = 0U;
  static const uint8_t _DST_END_S = 0U;

  // Member data
  const TimeSpan _Timezone;
  const bool _DstEnabled;
  RTC_DS1307 _Rtc;

  // Methods
  DateTime _utcToOfficial(DateTime UtcTime) const;
  bool _inDst(const DateTime &UtcTime) const;
  uint8_t _getLastDowOfMonth(uint16_t Year, uint8_t Month, uint8_t LastDom,
    uint8_t Dow) const;
};


#endif  // _CLOCK_H_