#include "config.h"
#include <assert.h>
#include <EEPROM.h>
#include "feeds.h"


/*
 *   Constructor.
 */
Feeds::Feeds():
  _NextMealId(_ID_NULL),
  _NextMealDealt(false),
  _SkipNextMeal(false)
{
  int Addr;
  uint8_t Id;

  // Initialize EEPROM address for each Meal object
  for (Id=0, Addr=_BASE_ADDR; Id<NUM_MEALS; Id++, Addr+=sizeof (Meal))
    _Meals[Id].setEepromAddress(Addr);
}


/*
 *   Initializes class, reading meals from EEPROM (or initializing the
 *  EEPROM if never used) and setting the next feed.
 *  Parameters:
 *  * Now: current time; next feed will be the first found after this time.
 */
void Feeds::init(const DateTime &Now)
{
  uint8_t Id;

  // Read magic record and check for validity
  if (EEPROM.read(_MAGIC_ADDR) != _MAGIC_NUMBER)
  {
    // Magic record has incorrect value -> initialize EEPROM data
    // First write meals data
    for (Id=0; Id<NUM_MEALS; Id++)
      _Meals[Id].saveEeprom();

    // Then write valid magic number
    EEPROM.write(_MAGIC_ADDR, _MAGIC_NUMBER);
  }
  else
  {
    // Magic number is correct, read saved meals from EEPROM
    for (Id=0; Id<NUM_MEALS; Id++)
      _Meals[Id].loadEeprom();
  }

  // Set next (first) meal
  _updateNext(Now);
}


/*
 *   Sets the EEPROM to an invalid state so the next boot will initialize it.
 */
void Feeds::resetEeprom()
{
  // Unset the magic number, e.g. writing its value inverted
  EEPROM.write(_MAGIC_ADDR, (uint8_t) ~_MAGIC_NUMBER);
}

/*
 *   Reset the object with a new current time. All meals until it will be
 *  skipped, next meal is re-calculated and the skip next meal status is also
 *  disabled.
 *  Parameters:
 *  * Now: current time; next feed will be the first found after this time.
 */
void Feeds::reset(const DateTime &Now)
{
  _SkipNextMeal = false;
  _NextMealDealt = false;

  // Calcule next meal again
  _NextMealId = _ID_NULL;
  _updateNext(Now);
}


/*
 *   Check whether it is time for a meal and returns the quantity to deliver.
 *  It also updates the object for the next meal.
 *  Parameters:
 *  * Now: current official time
 *  Returns: quantity of food to deliver from 1 to 9; 0 means no meal
 */
uint8_t Feeds::check(const DateTime &Now)
{
  uint8_t Quantity = 0U;
  const Meal *pMeal;

  // Is there a next feed?
  if (_NextMealId != _ID_NULL)
  {
    // First we check if the next meal needs to be reset. This happens when
    // this is the first call after (not matching time of) a meal that has
    // already been dealt with
    if (_NextMealDealt && !_Meals[_NextMealId].matchesTime(Now))
      // Reset changes _NextMealId, _NextMealDealt & _SkipNextMeal
      reset(Now);

    // Once next meal is reset (or not), check normally
    // Even when reset, we know that there is at least one meal available:
    // the very dealt meal in one week time, so assert it:
    assert(_NextMealId != _ID_NULL);

    pMeal = _Meals + _NextMealId;

    // Does next meal match the current day and time?
    if (pMeal->matchesTime(Now))
    {
      if (!_NextMealDealt)
      {
        // First time matching the meal time, deal with it
        _NextMealDealt = true;

        if (!_SkipNextMeal)
          Quantity = pMeal->getQuantity();
        else
        {
          // Reset skip for the next to this one we are skipping
          _SkipNextMeal = false;
          // skip this one: Quantity = 0
        }
      }
      // else: already checked this very minute, Quantity = 0
    }
    else
    {
      // Because we already checked this and did a reset() in that case
      assert(!_NextMealDealt);  

      // Times are different -> Quantity = 0
    }
  }
  // else: no meals active -> Quantity = 0

  return Quantity;
}


/*
 *   Instruct to skip the next programmed meal. No more than one next meal can
 *  be skipped. If no meals are enabled, ignore skip action.
 */
void Feeds::skipNext()
{
  // If there is no programmed next meal, do nothing
  if (_NextMealId != _ID_NULL)
    _SkipNextMeal = true;
}


/*
 *   Deactivates the skip for the next programmed meal.
 */
void Feeds::unskipNext()
{
  _SkipNextMeal = false;
}


/*
 *   Returns whether the next meal is marked to be skipped or not.
 */
bool Feeds::isSkippingNext() const
{
  return _SkipNextMeal;
}


/*
 *   Returns the time for next meal if any is programmed.
 *  Parameters:
 *  * pDotw: reutrn here the day of the week for the next meal (Sunday == 0).
 *  * pHour: return here the hour fraction for the next meal when not NEXT_NONE.
 *  * pMinute: return here the minute fraction for the next meal when not
 *    NEXT_NONE.
 *  Returns:
 *  * NEXT_NONE: there are no active meals.
 *  * NEXT_OK: returns time for the next meal, which will be served.
 *  * NEXT_SERVED: returns time for the last meal, which was already served
 *  * NEXT_SKIP: returns time for the next meal, but it is marked to be skipped.
 */
Feeds::Next_t Feeds::timeOfNext(uint8_t *pDotw, uint8_t *pHour,
  uint8_t *pMinute) const
{
  Next_t NextFeed;

  if (_NextMealId == _ID_NULL)
    NextFeed = NEXT_NONE;  // No next meal, all disabled
  else
  {
    if (_NextMealDealt)
      NextFeed = NEXT_SERVED;  // Meal already served but not updated
    else if (_SkipNextMeal)
      NextFeed = NEXT_SKIP;  // Skipping next meal
    else
      NextFeed = NEXT_OK;  // Next meal will be served

    // Fill time both for OK and SKIP
    _Meals[_NextMealId].getTime(pHour, pMinute);
    *pDotw = _NextMealDotw;
  }

  return NextFeed;
}


/*
 *   Returns a pointer to the requested meal object. It can be modified with
 *  its own methods. If that is done, we will need to be notified to update the
 *  _NextMealId.
 *  Parameters:
 *  * Id: meal identifier.
 */
Meal *Feeds::getMeal(uint8_t Id)
{
  assert(Id < NUM_MEALS);

  return _Meals + Id;
}


/*
 *   Saves meal data to EEPROM.
 *  Parameters:
 *  * Id: meal identifier.
 */
void Feeds::saveMeal(uint8_t Id)
{
  _Meals[Id].saveEeprom();
}


/*
 *   Updates class data after a change in one of the meals or in the last check
 *  time, which is taken as reference time.
 *  Parameters:
 *  * Now: current time in official 24h format.
 */
void Feeds::_updateNext(const DateTime &Now)
{
  DateTime NextMealTime;
  TimeSpan NextMealSpan;
  uint8_t Id;

Serial.println(F("Feeds:updateNext"));
  // When we have no next meal selected, find first active meal
  for (Id=0; _NextMealId==_ID_NULL && Id<NUM_MEALS; Id++)
    if (_Meals[Id].isEnabled())
      _NextMealId = Id;
Serial.print(F("First:"));
Serial.println(_NextMealId);
Serial.flush();

  // Continue loop through the rest of the meals and find the closest next one
  for (; Id<NUM_MEALS; Id++)
    // Do not compare with itself
    if (Id != _NextMealId)
      // If Id comes sooner than _NextMealId
      if (_Meals[_NextMealId].compare(_Meals[Id], Now, &NextMealSpan))
        // We have a new earliest Meal -> update
        _NextMealId = Id;
Serial.print(F("Next:"));
Serial.println(_NextMealId);
Serial.flush();

  // If we found a winner, calculate its DOTW
  if (_NextMealId != _ID_NULL)
  {
    // Casting needed because DateTime::operator+ is incorrectly declared
    // without const
    NextMealTime = (DateTime &) Now + NextMealSpan;
    _NextMealDotw = NextMealTime.dayOfTheWeek();
  }
}
