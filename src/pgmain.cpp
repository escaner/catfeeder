#include "pgmain.h"

/*
 *   Constructor.
 *  Paramters:
 *  * Lcd: reference to the lcd display that is being used.
 */
PgMain::PgMain(LiquidCrystal &Lcd):
  Page(Lcd),
  _State(StOk),
  _ManFeeding(false)
{
}


/*
 *   Starts a series of actions and events to draw this page in the display.
 *  It firs requests the time, and then the next meal time. Uses _State
 *  to track the sequence of events.
 *  Returns:
 *  * PageAction requesting the current official time or no action when
 *    the process was already unchained and not finished.
 */
PageAction PgMain::draw()
{
  // Only makes sense at StOk
  if (_State == StOk)
  {
    _State = StNeedTime;
    return PageAction(Action::AcNeedTime);
  }

  return PageAction();
}

/*
 *   Manage events received my main page.
 *  Parameters:
 *  * E: event data
 *  Returns:
 *  * PageAction with instructions on how to proceed.
 */
PageAction PgMain::event(const Event &E)
{
  switch (E.Id)
  {
  case Event::EvInit:
    // Initialize: display page for the first time
    return draw();

  case Event::EvSwitch:  // Switch has been pressed / released / something.
    // Are we manually feeding?
    if (_ManFeeding)
      // Discard all switch events except for this one
      if (E.Switch == Event::SwEvBackRelease)
      {
        // End manual feeding
        _ManFeeding = false;
        return PageAction(Action::ManualFeedEnd);
      }
      else
        // On any other switch, just continue feeding
        return PageAction(Action::ManualFeedContinue);

    // else (no manually feeding), check other switches
    switch (E.Switch)
    {
    case Event::SwEvBackPress:
      // Start manual feeding
      _ManFeeding = true;
      return PageAction(Action::ManualFeedStart);

    case Event::SwEvEnterPress:
      // Go to config page
      return PageAction(&_PgConfig);
    }
    // Default action for rest of switches
    break;

  case Event::EvTime:
    // Check if we requested it from draw()
    if (_State == StNeedTime)
      // Yes, advance state
      _State = StNeedNextMeal;
    // Either requested or because update, draw it
    _drawTime(E.Time);
    // Now request next meal data
    return PageAction(Action::AcNeedNextMeal);

  case Event::EvNextMeal:
    // Check if we requested it, unchained from draw()
    if (_State == StNeedNextMeal)
      // Yes, all data received
      _State = StOk;
    // Either requested or because update, draw it
    _drawNextMeal(E.NextMeal);
    // We don't need more data: no action
    break;
  }

  // Default: no action
  return PageAction();
}

void PgMain::_drawTime(const DateTime &Time) const
{
}
void PgMain::_drawNextMeal(const Event::NextMeal_t &NextMeal) const
{
}
