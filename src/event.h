#ifndef _EVENT_H_
#define _EVENT_H_

#include "config.h"
#include <Arduino.h>
#include <RTClib.h>
#include "feeds.h"


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
    EvTimeUtc,  // Carrying a DateTime class
    EvMeal,     // Carrying a Meal class
    EvNextMeal  // Carrying info about the next meal
  };

  // In case of EvSwitch, which switch has the user activated
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

  // Further data for EvNextMeal
  struct NextMeal_t
  {
    uint8_t Dotw;  // Range [0,6] being 0 Sunday (like DateTime class)
    uint8_t Hour;
    uint8_t Minute;
    Feeds::Next_t Status;
  };

  // Constructors
  Event() {}
  Event(EventId EvId): Id(EvId) {}
  
  // Copy constructor
  Event(const Event &Ev):
    Id(Ev.Id)
  {
#pragma GCC diagnostic push
// Disable: warning: enumeration value 'EvInit' not handled in switch
#pragma GCC diagnostic ignored "-Wswitch"

  switch (Id)
    {
    case EvSwitch:
      Switch = Ev.Switch;
      break;
    case EvTime:
    case EvTimeUtc:
      Time = Ev.Time;
      break;
    case EvMeal:
      pMeal = Ev.pMeal;
      break;
    case EvNextMeal:
      NextMeal = Ev.NextMeal;
      break;
    }

#pragma GCC diagnostic pop
  }


  /***************/
  /* Member data */
  /***************/

  EventId Id;
  union
  {
    SwitchEvent Switch;   // Used by EvSwitch
    DateTime Time;        // Used by EvTime, EvTimeUtc
    Meal *pMeal;          // Used by EvMeal
    NextMeal_t NextMeal;  // Used by EvNextMeal;
  };
};


#endif  // _EVENT_H_
