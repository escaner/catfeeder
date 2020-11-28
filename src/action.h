#ifndef _ACTION_H_
#define _ACTION_H_

#include <RTClib.h>
#include "meal.h"


/*
 *   This header defines class Action, which is used to communicate from the
 *  Display to the outside required actions such us changes in configuration,
 *  manual feed or to request further data.
 */
class Action
{
public:
  /**************/
  /* Data types */
  /**************/

  enum ActionId: uint8_t
  {
    AcNone=0,
    AcNeedTime,
    AcNeedNextMeal,
    AcNeedTimeUtc,
    AcNeedMeal,
    AcSetTime,
    AcSetMeal,
    ManualFeedStart,
    ManualFeedContinue,
    ManualFeedEnd,
    SkipMeal,
    UnskipMeal
  };

  struct Meal_t
  {
    uint8_t Id;         // Used by NeedMeal, SetMeal
    const Meal *pMeal;  // Used by SetMeal
  };

  // Constructors
  Action(): Id(AcNone) {}
  Action(ActionId AcId): Id(AcId) {}

  /***************/
  /* Member data */
  /***************/

  ActionId Id;
  union
  {
    Meal_t Meal;    // Used by NeedMeal, SetMeal
    DateTime Time;  // Used by SetTime
  };
};


#endif  // _ACTION_H_