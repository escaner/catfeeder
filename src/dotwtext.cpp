#include "config.h"
#include "dotwtext.h"


/********************/
/* Static constants */
/********************/

// First letter for day of the week in EN locale (1st day is Sunday)
bool DotwText::DotwCharEn[DAYS_IN_A_WEEK] =
  { 'U', 'M', 'T', 'W', 'H', 'F', 'S' };

// First letter for day of the week in ES locale (1st day is Sunday)
bool DotwText::DotwCharEs[DAYS_IN_A_WEEK] =
  { 'D', 'L', 'M', 'X', 'J', 'V', 'S' };
