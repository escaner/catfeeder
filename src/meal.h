#ifndef _MEAL_H_
#define _MEAL_H_

#include <RTClib.h>


/*
 *   Class to manage meal times and settings.
 */
class Meal
{
public:
  static const int EEPROM_ADDR_NULL = -1;
  static const uint8_t MAX_QUANTITY = 9U;
  static const uint8_t DEFAULT_HOUR = 8U;
  static const uint8_t DEFAULT_MINUTE = 0U;

  Meal(uint8_t Hour = DEFAULT_HOUR, uint8_t Minute = DEFAULT_MINUTE,
    int EepromAddress = EEPROM_ADDR_NULL);
  void setEepromAddress(int EepromAddress);
  void setTime(uint8_t Hour, uint8_t Minute);
  void setDotw(const bool pDotwArray[]);
  void setQuantity(uint8_t Quantity);
  void getTime(uint8_t *pHour, uint8_t *pMinute) const;
  void getDotw(bool pDotwArray[]) const;
  uint8_t getQuantity() const;
  bool isEnabled() const;
  bool compare(const Meal &OtherMeal, const DateTime &TimeRef) const;
  bool saveEeprom() const;
  bool loadEeprom();

protected:
  static const uint8_t _DAYS_IN_A_WEEK = 7U;
  static const uint8_t _HOURS_IN_A_DAY = 24U;
  static const uint8_t _MINUTES_IN_AN_HOUR = 60U;

  enum DotwPinMask: byte
  {
    MON = bit(0),
    TUE = bit(1),
    WED = bit(2),
    THU = bit(3),
    FRI = bit(4),
    SAT = bit(5),
    SUN = bit(6)
  };

  struct Meal_t
  {
    uint8_t Hour;      // 24h format hour fraction of the meal
    uint8_t Minute;    // Minute fraction of the time of the meal 
    byte Dotw;         // Days of the week the meal is enabled
    uint8_t Quantity;  // Food quantity to dispense in this meal. 0 = disabled
  };

  Meal_t _Meal;
  int _EepromAddress;  // Arduino EEPROM memory address assigned to this object

  uint8_t _nextOccurrenceDotw(uint8_t RefDotw, uint8_t RefHour,
    uint8_t RefMinute) const;
  TimeSpan _timeDifference(uint8_t RefDotw, uint8_t RefHour,
    uint8_t RefMinute) const;
  static inline uint8_t _incrDotw(uint8_t &Dotw, uint8_t Increment);
};


/******************/
/* Inline methods */
/******************/

/*
 *   Performs day of the week increment (addition) operation.
 *  Parameters:
 *  * Dotw: day of the week [0,6].
 *  * Increment: number of days to add.
 *  Returns the calculated new day of the week [0,6]
 */
uint8_t Meal::_incrDotw(uint8_t &Dotw, uint8_t Increment)
{
  Dotw += Increment;
  Dotw %= _DAYS_IN_A_WEEK;

  return Dotw;
}

#endif  // _MEAL_H_