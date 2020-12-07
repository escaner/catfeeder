#ifndef _DOTWTEXT_H_
#define _DOTWTEXT_H_

#include "config.h"
#include <Arduino.h>


/*
 *   Static class to translate day of the week indexes into Spansh text.
 *  Defaults to index 0 representing Sunday. For localization purposes,
 *  constants with the local first day of the week are provided.
 */
class DotwText
{
public:
  // Public Constants
  static const uint8_t DAYS_IN_A_WEEK = 7U;
  static const uint8_t FIRST_DOTW_ES = 1U;
  static const uint8_t FIRST_DOTW_EN = 0U;

  static bool DotwCharEs[DAYS_IN_A_WEEK];
  static bool DotwCharEn[DAYS_IN_A_WEEK];

  // Methods
  static inline uint8_t incr(uint8_t &DotwId, uint8_t Increment);
  static inline uint8_t add(uint8_t DotwId, uint8_t Increment);
};


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


#endif  // _DOTWTEXT_H_
