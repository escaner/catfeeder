#include "config.h"
#include "timer1.h"
#include <Arduino.h>


/********************/
/* Module constants */
/********************/

static const uint16_t _OCR = 3906U;  // Output compare register value
static const byte _TCCR_CTC_OCR1A = _BV(WGM12);  // CTC OCR1A mode for TCCR
static const byte _TCCR_1024 = _BV(CS12) | _BV(CS10);  // 1024 divider

static void (*_pIsrFunction)();


/********************/
/* Module functions */
/********************/


/*
 *   Define actual ISR to call the function we have stored.
 */
ISR(TIMER1_COMPA_vect)
{
  // Just call the function
  (*_pIsrFunction)();
}


/*
 *   Enables interrupts and configures the class to call the provided ISR
 *  function.
 *  Paramters:
 *  * pIsrFunction: pointer to functionISR  to call on interrupt
 */
void enableIsr(void (*pIsrFunction)())
{
  // Set new ISR function
  _pIsrFunction = pIsrFunction;

  noInterrupts();

  TCCR1A = 0;
  TCCR1B = _TCCR_CTC_OCR1A | _TCCR_1024;  // CTC mode for OCR1A & 1024 divider
  OCR1A = _OCR;  // 16MHz / 1024 / 4Hz
  TIMSK1 |= _BV(OCIE1A);  // Enable match interrupts on Output Compare A

  interrupts();
}


/*
 *   Disables interrupts.
 */
void disableIsr()
{
  noInterrupts();

  TIMSK1 &= ~_BV(OCIE1A);  // Disable timer match interrupts on OC A
  TCCR1B = 0;  // Reset operation modes
  TIFR1 &= ~_BV(OCF1A);  // Clear a possible pending interrupt flag

  interrupts();

  // Disable ISR function
  _pIsrFunction = nullptr;
}
