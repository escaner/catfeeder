#include <assert.h>
#include <EEPROM.h>
#include "feeds.h"


/*
 *   Constructor.
 */
Feeds::Feeds():
  _SkipNextMeal(false),
  _NextMealId(_ID_NULL)
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

  // Initialize last check time with current time and set next (first) meal
  _LastCheck = Now;
  _updateNext();
}


/*
 *   Check whether it is time for a meal and returns the quantity to deliver.
 *  Parameters:
 *  * Now: current official time
 *  Returns: quantity of food to deliver from 1 to 9; 0 means no meal
 */
uint8_t Feeds::check(const DateTime &Now)
{
  uint8_t Quantity;

  // Is there a next feed?
  if (_NextMealId != _ID_NULL)
  {
    // Check that deliver time has passed
    // if (bit(Now.dayOfTheWeek()) & _Meals[_NextMealId
     Check that meal should not be skipped or skip it
  }
  else
    // No next feed time programmed -> do not deliver food
    Quantity = 0U;

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
void Feeds::skipNext()
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
 *  * pHour: return here the hour fraction for the next meal when not NEXT_NONE.
 *  * pMinute: return here the minute fraction for the next meal when not
 *    NEXT_NONE.
 *  Returns:
 *  * NEXT_NONE: there are no active meals.
 *  * NEXT_OK: returns time for the next meal, which will be served.
 *  * NEXT_SKIP: returns time for the next meal, but it is marked to be skipped.
 */
Feeds::Next_t Feeds::timeOfNext(uint8_t *pHour, uint8_t *pMinute) const
{
  Next_t NextFeed;

  if (_NextMealId == _ID_NULL)
    NextFeed = NEXT_NONE;  // No next meal, all disabled
  else
  {
    if (_SkipNextMeal)
      NextFeed = NEXT_SKIP;  // Skipping next meal
    else
      NextFeed = NEXT_OK;  // Next meal will be served

    // Fill time both for OK and SKIP
    _Meals[_NextMealId].getTime(pHour, pMinute);
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
Meal Feeds::getMeal(uint8_t Id) const
{
  assert(Id < NUM_MEALS);

  return _Meals[Id];
}


void setMeal(const DateTime &Now, uint8_t Id, const Meal &NewMeal)



/*
 *   Updates class data after a change in one of the meals or in the last check
 *  time, which is taken as reference time.
 *  Parameters:
 *  * Now: current time in official 24h format.
 */
void Feeds::_updateNext()
{
  uint8_t Id;

  // When we have no next meal selected, find first active meal
  for (Id=0; _NextMealId==_ID_NULL && Id<NUM_MEALS; Id++)
    if (_Meals[Id].isEnabled())
      _NextMealId = Id;

  // Continue loop through the rest of the meals and find the closest next one
  for (; Id<NUM_MEALS; Id++)
    // Do not compare with itself
    if (Id != _NextMealId)
      // If Id comes sooner than _NextMealId
      if (_Meals[_NextMealId].compare(_Meals[Id], _LastCheck))
        // We have a new earliest Meal -> update
        _NextMealId = Id;
}
