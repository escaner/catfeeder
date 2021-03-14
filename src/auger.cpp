#include "config.h"
#include <assert.h>
#include "auger.h"


/*
 *   Constructor. Initializes class and stepper motor EasyDriver controller.
 *  Parameters:
 *  * PinStep: STEP pin in controller, marking the steps of the motor.
 *  * PinDir: DIR pin in controller, determining direction of the movement.
 *  * PinMs1: MS1 pin in controller, determining step size with MS2.
 *  * PinMs2: MS2 pin in controller, determining step size with MS1.
 *  * PinEnable: ENABLE pin in controller, activating enery to the motor.
 *  * Rpm: movement speed in revolutions per second. MAX=120.
 *  * EighthRevsPerQtyUnit: Eighth revolutions per quantity unit. One full
 *    revolution is 8 eighths. Note that Quantity*EighthRevsPerQtyUnit<256
 *    or an overflow condition will happen.
 */
Auger::Auger(uint8_t PinStep, uint8_t PinDir, uint8_t PinMs1, uint8_t PinMs2,
    uint8_t PinEnable, uint8_t Rpm, uint8_t EighthRevsPerQtyUnit,
    uint8_t EighthRevsPerBackup):
  _PinStep(PinStep),
  _PinDir(PinDir),
  _PinMs1(PinMs1),
  _PinMs2(PinMs2),
  _PinEnable(PinEnable),
  _EighthRevsPerQtyUnit(EighthRevsPerQtyUnit),
  _StepsContinuousFeed(EighthRevsPerQtyUnit * _STEPS_PER_8REV),
  _StepsBackup(EighthRevsPerBackup * _STEPS_PER_8REV),
  // Calculate ms per half step
  _HalfStepTime( (unsigned int)
    ((60UL * 1000000UL) /
    (uint32_t(Rpm) * uint32_t(_STEPS_PER_REV * 2U))))
{
  // Avoid burning the motor!
  assert(Rpm <= 120U);

  // Prepare Arduino to control EasyDriver
  pinMode(PinStep, OUTPUT);
  pinMode(PinDir, OUTPUT);
  pinMode(PinMs1, OUTPUT);
  pinMode(PinMs2, OUTPUT);
  pinMode(PinEnable, OUTPUT);

  // Prepare EasyDriver controller
  digitalWrite(PinEnable, HIGH);
  digitalWrite(PinStep, LOW);
  digitalWrite(PinDir, _DIR_FORWARD);
  digitalWrite(PinMs1, HIGH);
  digitalWrite(PinMs2, HIGH);
}


/*
 *   Synchronously moves the auger to feed.
 *  Paramters:
 *  * Quantity: size of the meal. Note that Quantity*Hrqu<256
 *    or an overflow condition will happen.
 */
void Auger::feed(uint8_t Quantity) const
{
  uint16_t Steps;
  
  // Calculate number of steps for this meal
  Steps = uint16_t(_EighthRevsPerQtyUnit) * _STEPS_PER_8REV;
  //  Steps = uint16_t(Quantity * _EighthRevsPerQtyUnit) * _STEPS_PER_8REV;


  // Engage the motor
  _enableMotor();

  // Deliver the food turning the motor and jiggling every quantity unit
  while (Quantity--)
  {
    _jiggle();
    _turnMotor(Steps);
  }
 
  // Save power
  _disableMotor();
}


/*
 *   Starts a semi-synchronous feed.
 */
void Auger::startFeeding() const
{
  _enableMotor();
  keepFeeding();
}


/*
 *   Continues a semi-synchronous feed, jiggling on each call
 */
void Auger::keepFeeding() const
{
  _jiggle();
  _turnMotor(_StepsContinuousFeed);
}


/*
 *   Ends a semi-synchronous feed. Does not deliver actual food.
 */
void Auger::endFeeding() const
{
  _disableMotor();
}


/*
 *   Powers up the motor.
 */
void Auger::_enableMotor() const
{
  digitalWrite(_PinEnable, LOW);
}


/*
 *   Powers down the motor.
 */
void Auger::_disableMotor() const
{
  digitalWrite(_PinEnable, HIGH);
}


/*
 *   Turns the motor (auger)
 *   Parameters:
 *   * NumSteps: how many steps the motor will be turned
 */
void Auger::_turnMotor(uint16_t NumSteps) const
{
  while (--NumSteps)
  {
    // Perform one full step
    digitalWrite(_PinStep, HIGH);
    delayMicroseconds(_HalfStepTime);
    digitalWrite(_PinStep, LOW);
    delayMicroseconds(_HalfStepTime);
  }
}


/*
 *  Jiggles the auger back and forth to prevent it getting stuck.
 */
void Auger::_jiggle() const
{
  digitalWrite(_PinDir, _DIR_BACKWARD);
  _turnMotor(_StepsBackup);
  digitalWrite(_PinDir, _DIR_FORWARD);
  _turnMotor(_StepsBackup);
}
