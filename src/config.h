/*
 *   Define constants for general program configuration.
 */

// Total number of meals that can be configured
static const uint8_t NUM_MEALS = 8U;

// Local time = UTC + TIMEZONE_DIFF (in minutes)
static const int32_t TIMEZONE_DIFF = 60;

// Whether to enable or not Daylight Saving Time periods
static const bool ENABLE_DST = true;

// Auger speed in revolutions per minute
static const uint8_t AUGER_RPM = 15U;

// How many 1/8th of a revolution should the auger be rotated for each meal
// quantity unit
static const uint8_t AUGER_EIGHTH_REVS_PER_MEAL_QTY = 2U;