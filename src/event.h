#ifndef _EVENT_H_
#define _EVENT_H_

#include <RTClib.h>
#include "meal.h"


/*
 *   This header defines class Event, which is used to communicate to the
 *  Display events such as button actions or just requested data.
 */
class Event
{
public:
  /**************/
  /* Data types */
  /**************/

  // Type of the event structure
  enum EventId: uint8_t
  {
    EvInit,     // Initialize, first draw
    EvSwitch,   // Switch (button) event
    EvTime,     // Carrying a DateTime class
    EvMeal,     // Carrying a Meal class
    EvNextMeal  // Carrying info about the next meal
  };

  // Event that can be returned regarding the switch user input
  enum SwitchEvent: uint8_t
  {
    SwEvNone=0,        // No input switch event
    SwEvSelectCcw,     // Counterclockwise event on the Select encoder
    SwEvSelectCw,      // Clockwise event on the Select encoder
    SwEvEnterPress,    // Enter button press event (Select button pressed)
    SwEvEnterRelease,  // Enter button release event (Select button released)
    SwEvBackPress,     // Back button press event
    SwEvBackRelease    // Back button release event
  };

  struct NextMeal_t
  {
    uint8_t Dotw;
    uint8_t Hour;
    uint8_t Minute;
    bool Skip;
  };


  /***************/
  /* Member data */
  /***************/

  EventId Id;
  union
  {
    SwitchEvent Switch;   // Used by EvSwitch
    DateTime Time;        // Used by EvTime
    Meal *pMeal;          // Used by EvMeal
    NextMeal_t NextMeal;  // Used by EvNextMeal;
  };
};


#endif  // _EVENT_H_
