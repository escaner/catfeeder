#include "config.h"
#include <assert.h>
#include <EEPROM.h>
#include "meal.h"
#include "dotwutil.h"


/***********/
/* Methods */
/***********/


/*
 *   Constructor. Initializes EEPROM address.
 *  Parameters:
 *  * Hour: 24h format hour
 *  * Minute: minute fraction of the time
 *  * EepromAddress: EEPROM load/save address for this object.
 */
Meal::Meal(uint8_t Hour, uint8_t Minute, int EepromAddress):
  _EepromAddress(EepromAddress)
{
  _Meal.Hour = Hour;
  _Meal.Minute = Minute;
  _Meal.Dotw = 0x00;
  _Meal.Quantity = 0U;
}


/*
 *   Initializes EEPROM address for load and saves of this object.
 *  Parameters:
 *  * EepromAddress: EEPROM load/save address for this object.
 */
void Meal::setEepromAddress(int EepromAddress)
{
  _EepromAddress = EepromAddress;
}


/*
 *   Sets time (hour & minute) for this object meal.
 *  Parameters:
 *  * Hour: 24h format hour
 *  * Minute: minute fraction of the time
 */
void Meal::setTime(uint8_t Hour, uint8_t Minute)
{
  assert(Hour < 24);
  assert(Minute < 60);

  _Meal.Hour = Hour;
  _Meal.Minute = Minute;
}


/*
 *   Sets days of the week for this meal. Converts from external array into
 *  bits.
 *  Parameters
 *  * pDotw: 7 day array, one for each day of the week, starting on Sunday.
 */
void Meal::setDotw(const bool pDotwArray[])
{
  int8_t Idx = (int8_t) DotwUtil::DAYS_IN_A_WEEK;
  byte Dotw = 0U;

  // Pack array of 7 bools into a byte
  while (--Idx >= 0)
  {
    // Current bit in loop is the least significant in Dotw
    // Set bit when true
    if (pDotwArray[Idx])
      Dotw |= 0b1;

    // Do not shift least significant (final) bit
    if (Idx > (int8_t) 0)
      Dotw <<= 1;
  }

  // Set
  _Meal.Dotw = Dotw;
}


/*
 *   Sets quantity of food to dispense in this meal.
 *  Parameters:
 *  * Quantity: quantity of food to dispense, 0 means disabled.
 */
void Meal::setQuantity(uint8_t Quantity)
{
  assert(Quantity < MAX_QUANTITY);

  _Meal.Quantity = Quantity;
}


/*
 *   Returns the time of the meal.
 *  Parameters:
 *  * pHour: hour of the meal will be returned throgh this pointer (24H format).
 *  * pMinute: minute of the meal will be returned throgh this pointer.
 */
void Meal::getTime(uint8_t *pHour, uint8_t *pMinute) const
{
  *pHour = _Meal.Hour;
  *pMinute = _Meal.Minute;
}


/*
 *   Returns days of the week for this meal. Converts internal bits to external
 *  array of bool. First day of the week is Sunday.
 *  Parameters
 *  * pDotw: buffer where the array will be stored.
 */
void Meal::getDotw(bool pDotwArray[]) const
{
  uint8_t Idx;
  byte Dotw = _Meal.Dotw;

  // Unack byte of 7 DOTW bits into array of 7 bools.
  for (Idx = 0U; Idx<DotwUtil::DAYS_IN_A_WEEK; Idx++)
  {
    // Get bit and store it as bool
    pDotwArray[Idx] = (bool) (Dotw & 0b1);

    // Discard current bit and ready next one
    Dotw >>= 1;
  }
}


/*
 *   Returns the quantity of food to dispense in this meal. 0 means it is
 *  disabled.
 */
uint8_t Meal::getQuantity() const
{
  return _Meal.Quantity;
}


/*
 *   Returns whether this meal is enabled (quantity > 0 and programmed for
 *  at least one day in the week) or not.
 */
bool Meal::isEnabled() const
{
  return _Meal.Quantity>0 && _Meal.Dotw!=0x00;
}


/*
 *   Compares this meal with another one and returns which one is the next
 *  in reference to a given time.
 *   Note: assumes that both meals are enabled!
 *  Parameters:
 *  * OtherMeal: this Meal will be compared with Other Meal
 *  * TimeRef: absolute time reference for the next meal
 *  * Span: return here the time from TimeRef to the first meal of the two
 *  Returns: true iff OtherMeal is earlier than this Meal; false if this
 *  Meal is earlier or at the same time.
 */
bool Meal::compare(const Meal &OtherMeal, const DateTime &TimeRef,
  TimeSpan *pSpan) const
{
  bool OtherFirst;  // Whether OtherMeal arrives before this Meal
  uint8_t RefDotw, RefHour, RefMinute;  // Reference day of the week & time
  TimeSpan ThisSpan, OtherSpan;

  // This makes no sense when meals have no day of the week enabled
  assert(_Meal.Dotw != 0x00);
  assert(OtherMeal._Meal.Dotw != 0x00);

  // Get reference time stuff
  RefDotw = TimeRef.dayOfTheWeek();
  RefHour = TimeRef.hour();
  RefMinute = TimeRef.minute();

  // Get time to next occurrence of each meal
  ThisSpan = timeDifference(RefDotw, RefHour, RefMinute);
  OtherSpan = OtherMeal.timeDifference(RefDotw, RefHour, RefMinute);

  // Which one is earlier?
  if (ThisSpan.totalseconds() > OtherSpan.totalseconds())
  {
    // OtherMeal is earlier
    *pSpan = OtherSpan;
    OtherFirst = true;
  }
  else
  {
    // This Meal is earlier
    *pSpan = ThisSpan;
    OtherFirst = false;
  }

  return OtherFirst;
}


/*
 *   Calculates the time from the reference time and day of the week to the
 *  next occurrence of this meal.
 *   The result is undefined if this meal has no day of the week enabled.
 *  Parameters:
 *  * RefDotw: day of the week of the reference time.
 *  * RefHour: hour fraction of the reference time.
 *  * RefMinute: minute fraction of the reference time.
 *  Return: a TimeSpan with the difference in time. It will always be positive
 *  because this meal is assumed to occur later then the reference time.
 */
TimeSpan Meal::timeDifference(uint8_t RefDotw, uint8_t RefHour,
  uint8_t RefMinute) const
{
  uint8_t MealDotw, MealHour, MealMinute;
  uint8_t Days, Hours, Minutes;

  // Get day of the week for next occurrence
  MealDotw = _nextOccurrenceDotw(RefDotw, RefHour, RefMinute);
  MealHour = _Meal.Hour;
  MealMinute = _Meal.Minute;

  // Minute difference
  if (MealMinute < RefMinute)
  {
    // Avoid negative overflow, take an hour
    MealMinute += _MINUTES_IN_AN_HOUR;
    RefHour++;
  }
  Minutes = MealMinute - RefMinute;

  // Hour difference
  if (MealHour < RefHour)
  {
    // Avoid negative overflow, take a day
    MealHour += _HOURS_IN_A_DAY;
    RefDotw++;
  }
  Hours = MealHour - RefHour;

  // Day difference
  if (MealDotw < RefDotw)
    // Avoid negative overflow, delay meal a week
    MealDotw += DotwUtil::DAYS_IN_A_WEEK;
  Days = MealDotw - RefDotw;
    
  return TimeSpan(Days, Hours, Minutes, 0U);
}


/*
 *   Checks whether this Meal programmed time and days of the week match
 *  a reference Time.
 *  Parameters:
 *  * Time: reference time for the comparison.
 *  Returns:
 *  * true: iff the Meal is enabled for the day of the week, hour
 *  and minute of Time.
 *  * false: otherwise
 */
bool Meal::matchesTime(const DateTime &Time) const
{
  return
    Time.hour() == _Meal.Hour &&
    Time.minute() == _Meal.Minute &&
    bitRead(_Meal.Dotw, Time.dayOfTheWeek());
}


/*
 *   Saves current object into Arduino EEPROM memory at the address assigned
 *  to this object.
 *   Return: true iff the address has not been initialized or is not valid.
 */
bool Meal::saveEeprom() const
{
  // Check that we have a valid EEPROM address
  if (_EepromAddress < 0)
    return true;

  // Save meal data into Arduino EEPROM
  EEPROM.put(_EepromAddress, _Meal);

  return false;
}


/*
 *   Reads current object from Arduino EEPROM memory at the address assigned
 *  to this object.
 *   Return: true iff the address has not been initialized or is not valid.
 */
bool Meal::loadEeprom()
{
  // Check that we have a valid EEPROM address
  if (_EepromAddress < 0)
    return true;

  // Read meal data from Arduino EEPROM
  EEPROM.get(_EepromAddress, _Meal);

  return false;
}


/*
 *   Finds the day of the week for the next occurrence of this meal
 *  after (but not equal) the reference time and day of the week.
 *  Parameters:
 *  * RefDotw: reference day of the week with range [0,6]
 *  * RefHour: reference hour
 *  * RefMinute: reference minute
 *  Returns: day of the week of the next occurrence of this meal. If the meal
 *  is not enabled for any day of the week, it will return RefDotw.
 */
uint8_t Meal::_nextOccurrenceDotw(uint8_t RefDotw, uint8_t RefHour,
  uint8_t RefMinute) const
{
  uint8_t MealDotw;

  // Start with reference day of the week
  MealDotw = RefDotw;

  // If this meal has reference day of the week enabled and time is later
  // (but not equal) we have found its next occurrence...
  if (!(bitRead(_Meal.Dotw, MealDotw) &&
      (_Meal.Hour>RefHour || (_Meal.Hour==RefHour && _Meal.Minute>RefMinute))))
  {
    // ... otherwise, keep looking up to one week later
    for (DotwUtil::incr(MealDotw, 1); MealDotw!=RefDotw;
         DotwUtil::incr(MealDotw, 1))
      // If this day of the week is enabled, any time is good: found
      if (bitRead(_Meal.Dotw, MealDotw))
        break;

    // If we reach this point exiting the loop by its condition, MealDotw will
    // be again RefDotw. Two possibilities:
    // 1. The DOTW is enabled in this meal but was a previous time the same day.
    // 2. No day of the week is enabled for this meal, we just return RefDotw.
  }

  return MealDotw;
}
