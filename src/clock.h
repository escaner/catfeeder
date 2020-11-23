#ifndef _CLOCK_H_
#define _CLOCK_H_

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
  static const TimeSpan DST_DIFFERENCE;
  static const uint8_t DAYS_IN_A_WEEK;
  static const uint8_t DST_START_MONTH;
  static const uint8_t DST_START_MONTH_LAST_DAY;
  static const uint8_t DST_START_DOW;
  static const uint8_t DST_START_H;
  static const uint8_t DST_START_M;
  static const uint8_t DST_START_S;
  static const uint8_t DST_END_MONTH;
  static const uint8_t DST_END_MONTH_LAST_DAY;
  static const uint8_t DST_END_DOW;
  static const uint8_t DST_END_H;
  static const uint8_t DST_END_M;
  static const uint8_t DST_END_S;

  // Member data
  TimeSpan _Timezone;
  bool _DstEnabled;
  RTC_DS1307 _Rtc;

  // Methods
  DateTime _utcToOfficial(DateTime UtcTime) const;
  bool _inDst(const DateTime &UtcTime) const;
  uint8_t _getLastDowOfMonth(uint16_t Year, uint8_t Month, uint8_t LastDom,
    uint8_t Dow) const;
};


#endif  // _CLOCK_H_