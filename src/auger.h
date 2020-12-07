#ifndef _AUGER_H_
#define _AUGER_H_

#include "config.h"
#include <Arduino.h>


/*
 *   Class to interface with the EasyDriver controller and a stepper motor
 *  attached and connected to the auger that dispenses the food.
 */
class Auger
{
public:
  Auger(uint8_t PinStep, uint8_t PinDir, uint8_t PinMs1, uint8_t PinMs2,
    uint8_t PinEnable, uint8_t Rpm, uint8_t EighthRevsPerQtyUnit);

  void feed(uint8_t Quantity) const;
  void startFeeding() const;
  void keepFeeding() const;
  void endFeeding() const;

protected:
  static const uint16_t _STEPS_PER_REV = 200U;
  static const uint16_t _STEPS_PER_8REV = _STEPS_PER_REV / 8U;  // 1/8 of a rev
  static const uint16_t _STEPS_CONTINUOUS_FEED = _STEPS_PER_8REV;
  static const uint8_t _DIRECTION = LOW;

  const uint8_t _PinStep;
  const uint8_t _PinDir;
  const uint8_t _PinMs1;
  const uint8_t _PinMs2;
  const uint8_t _PinEnable;
  const uint8_t _EighthRevsPerQtyUnit;
  const unsigned int _HalfStepTime;  // 1/2 of a step in microseconds

  void _enableMotor() const;
  void _disableMotor() const;
  void _turnMotor(uint16_t NumSteps) const;
};

#endif  // _AUGER_H_