#ifndef _FEEDS_H_
#define _FEEDS_H_

#include "config.h"
#include <Arduino.h>
#include <RTClib.h>
#include "config.h"
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
  enum Next_t: int8_t
  {
    NEXT_NONE = -1,
    NEXT_OK   = 0,
    NEXT_SERVED = 1,
    NEXT_SKIP = 2
  };

  Feeds();
  void init(const DateTime &Now);
  void reset(const DateTime &Now);
  void resetEeprom();
  uint8_t check(const DateTime &Now);
  void skipNext();
  void unskipNext();
  bool isSkippingNext() const;
  Next_t timeOfNext(uint8_t *pDotw, uint8_t *pHour, uint8_t *pMinute) const;
  Meal *getMeal(uint8_t Id);
  void saveMeal(uint8_t Id);

protected:
  static const uint8_t _ID_NULL = UINT8_MAX;
  static const uint8_t _MAGIC_NUMBER = 0b11100010;
  static const int _MAGIC_ADDR = 0;
  static const int _BASE_ADDR = 1;

  Meal _Meals[NUM_MEALS];
  uint8_t _NextMealId;    // Id if the next programmed meal
  uint8_t _NextMealDotw;  // Day of the week for the If Meal
  bool _NextMealDealt;    // Whether _NextMealId has already been fed/skipped
  bool _SkipNextMeal;     // Whether to skip the next meal

  void _updateNext(const DateTime &Now);
};

#endif  // _FEEDS_H_