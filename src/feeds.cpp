#include <assert.h>
#include "feeds.h"


/*
 *   Constructor.
 */
Feeds::Feeds():
  _SkipNextMeal(false),
  _NextMealId(ID_NULL)
{
  int Addr;
  uint8_t Id;

  // Initialize EEPROM address for each Meal object
  for (Id=0, Addr=BASE_ADDR; Id<NUM_MEALS; Id++, Addr+=sizeof (Meal))
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
  if (_NextMealId != ID_NULL)
  {
    // Check that deliver time has passed
    // if (bit(Now.dayOfTheWeek()) & _Meals[_NextMealId
  }
  else
    // No next feed time programmed -> do not deliver food
    Quantity = 0U;

  return Quantity;
}


/*
 *   Instruct to skip the next programmed meal. No more than one next meal can
 *  be skipped.
 */
void Meals::skipNext()
{
  // If there is no programmed next meal, do nothing
  if (_NextMealId != ID_NULL)
    _SkipNextMeal = true;
}


bool nextTime(const DateTime &Now, uint8_t *pHour, uint8_t *pMin) const
{
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

  return _Meals+Id;
}

/*
 *   Updates class data after a change in one of the meals.
 *  Parameters:
 *  * Now: current time in official 24h format.
 */
void Feeds::_updateNext(const DateTime &Now)
{
  uint8_t Id;

  // When we have no next meal selected, find first active meal
  for (Id=0; _NextMealId==_ID_NULL && Id<NUM_MEALS; Id++)
    if (_Meals[Id].isEnabled())
      _NextMealId = Id;

  // Continue through the rest of the meals and find the closest next one
  for (; Id<NUM_MEALS; Id++)
Meal.compare()
// Do not compare with itself!!!
}
