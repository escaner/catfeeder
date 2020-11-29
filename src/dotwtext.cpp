#include "dotwtext.h"

/**************************************/
/* Static constants with localization */
/**************************************/

const char DotwText::DotwCharEn[DAYS_IN_A_WEEK] =
  { 'U', 'M', 'T', 'W', 'H', 'F', 'S' };

const char DotwText::DotwCharEs[DAYS_IN_A_WEEK] =
  { 'D', 'L', 'M', 'X', 'J', 'V', 'S' };


/******************/
/* Inline methods */
/******************/

/*
 *   Performs day of the week increment (addition) operation modifying the
 *  original paramter value.
 *  Parameters:
 *  * DotwId: day of the week [0,6].
 *  * Increment: number of days to add.
 *  Returns the calculated new day of the week [0,6]
 */
inline uint8_t DotwText::incr(uint8_t &DotwId, uint8_t Increment)
{
  DotwId += Increment;
  DotwId %= DAYS_IN_A_WEEK;

  return DotwId;
}


/*
 *   Performs day of the week increment (addition) operation keeping the original
 *  parameter value.
 *  Parameters:
 *  * DotwId: day of the week [0,6].
 *  * Increment: number of days to add.
 *  Returns the calculated new day of the week [0,6]
 */
inline uint8_t DotwText::add(uint8_t DotwId, uint8_t Increment)
{
  DotwId += Increment;
  DotwId %= DAYS_IN_A_WEEK;

  return DotwId;
}