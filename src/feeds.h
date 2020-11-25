#ifndef _FEEDS_H_
#define _FEEDS_H_

#include <RTClib.h>
#include "meal.h"


/*
 *   Class to manage feed times and related events. All times used by this
 *  this class must be homogeneous: using official times.
 *   This class stores a magic record identifier to validate that data in the
 *  EEPROM is valid. If the record does not match, it will overwrite the
 *  EEPROM with default values.
 */
class Feeds
{
public:
  static const uint8_t NUM_MEALS = 4U;

  Feeds();
  void init(const DateTime &Now);
  uint8_t check(const DateTime &Now);
  void skipNext();
  bool nextTime(const DateTime &Now, uint8_t *pHour, uint8_t *pMin) const;
  Meal getMeal(uint8_t Id) const;
  void setMeal(const DateTime &Now, uint8_t Id, const Meal &NewMeal);


protected:
  static const uint8_t _ID_NULL = UINT8_MAX;
  static const uint8_t _MAGIC_NUMBER = 0b11100010;
  static const int _MAGIC_ADDR = 0;
  static const int _BASE_ADDR = 1;


  Meal _Meals[NUM_MEALS];
  bool _SkipNextMeal;
  uint8_t _NextFeedId;
  DateTime _LastCheck;

  void _updateNext(const DateTime &Now);
};

#endif  // _FEEDS_H_