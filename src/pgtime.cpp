#include "pgtime.h"


/********************/
/* Member constants */
/********************/

// Maximum number of days per month
const uint8_t PgTime::_MAX_DAY[_MAX_MONTH+1] =
  { 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U };

  // Static tags in the display
const char PgTime::_TIME_TAG[] = "TIEMPO";
const char PgTime::_DATE_TAG[] = "UTC";


/***********/
/* Methods */
/***********/

