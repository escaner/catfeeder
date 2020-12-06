#ifndef _CONFIG_H_
#define _CONFIG_H_

#define NDEBUG
#define __ASSERT_USE_STDERR

#include <Arduino.h>


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

// FEED interval check in ms
static const unsigned long FEED_CHECK_INTERVAL = 10000UL;

// Main page time refresh
static const unsigned long TIME_UPDATE_INTERVAL = 1000UL;

// Iterations reading switch panel per lopp() call
static const uint16_t SWITCH_LOOP_COUNT = 600U;

// Time sice a meal is served to update the LCD next meal information in ms
static const uint16_t MEAL_UPDATE_DELAY = 5U * 60U * 1000U;

/* Display */
//static const uint8_t DISPLAY_ROWS = 2U;
//static const uint8_t DISPLAY_COLS = 16U;


#endif  // _CONFIG_H_
