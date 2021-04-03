https://github.com/escaner/catfeeder

Catfeeder is an Arduino project to build am automatic cat feeder that
allows to program several feeding times so your mascot keeps getting food.

3D printed parts can be found here:
https://www.thingiverse.com/thing:4815584

The core of the system is an Arduino Nano or clone (small Arduino Uno
compatible) with a stepper motor connected to an auger, a display,
some buttons and a real time clock to keep the time in case the power
temporarily fails.

This is the list of electronic parts for the project:

* 1 x Arduino Nano compatible microcontroller
* 1 x SchmalzHaus EasyDriver: stepper motor controller
* 1 x Hanpose 17HS3430 stepper motor (12V)
* 1 x 1602 LCD display (without I2C module)
* 1 x Potentiometer 5 (or 10) KOhm for display brightness
* 1 x Potentiometer 10 KOhm for display contrast
* 1 x rotary encoder with pushbutton
* 1 x temporary pushbutton
* 1 x Robotdyn DS1307 I2C RTC module with battery as real time clock and backup
* 1 x Robotdyn LM2596 DC-DC Step-down Adjustable Power Supply Module (set
  to 5V output)
* 1 x DC barrel jack for 12V DC input required by the motor
* 1 x 12V 1.2A DC power supply (brick wall or as you prefer)

Note that in order for the reset function to correctly work, the Arduino Nano
needs to have the new Optiboot bootloader: the old one bugs out on reset
with the watchdog library.

More info can be found in the doc directory, including schematics and
pictures.

Other items used:
* 3D printer for case and parts
* Drill and 3mm bit to make hole in the T for the screw where the motor mount
  will lean
* PVC 50mm T pipe for water
* Epoxy two component glue (to fix T, bottom and front sides of case)
* Hot glue gun (to fix motor mount to T, left and right sides of case and
  also Arduino, motor controller and RTC to the case)
* M3 screws and nuts (to fix LCD, motor to its mount, auger to motor, screw in
  T and switched power supply)

Software features:
* Eight (can be increased up to 10) programmable feed times (including time,
  days of the week and quantity). Stored in the Arduino EEPROM (not lost on
  power off).
* Real Rime Clock with battery to keep time in case of power loss.
* Possibility of skipping next feed
* Time stored in UTC format for automatic DST changes (CET timezone
  programmed, can be easily changed in config.h).
* Day of the week is currently in Spanish, but not hard to change to English,
  code already prepared for that.
* Customizable through config.h and catfeeder.ino constants.
* Main display shows current date and time and also next feed day and time
* Manual feed function

This software needs my libraries REncoder and Switch:
https://github.com/escaner/REncoder
https://github.com/escaner/Switch

This project is based on Kitlaan and dodgey99 projects, seen here:
http://www.thingiverse.com/thing:27854
https://www.instructables.com/Automatic-Arduino-Powered-Pet-Feeder/


Copyright (C) 2020-2021, Óscar Laborda

This file is part of Catfeeder software.

Catfeeder is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Catfeeder is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Catfeeder.  If not, see <https://www.gnu.org/licenses/>.
