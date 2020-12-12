https://github.com/escaner/catfeeder

Catfeeder is an Arduino project to build am automatic cat feeder that
allows to program several feeding times so your mascot keeps getting food.

The core of the system is an Arduino Nano or clone (small Arduino Uno
compatible) with a stepper motor connected to an auger, a display,
some buttons and a real time clock to keep the time in case the power
temporarily fails.

This is the list of electronic parts for the project:

* 1 x Arduino Nano compatible microcontroller
* 1 x SchmalzHaus EasyDriver: stepper motor controller
* 1 x Hanpose 17HS3430 stepper motor
* 1 x 1602 LCD display
* 1 x Potentiometer 5 KOhm for display brightness
* 1 x Potentiometer 10 KOhm for display contrast
* 1 x rotary encoder with pushbutton
* 1 x temporary pushbutton
* 1 x Robotdyn DS1307 I2C RTC module with battery as real time clock and backup

Note that in order for the reset function to correctly work, the Arduino Nano
needs to have the new Optiboot bootloader: the old one bugs out on reset
with the watchdog library.

More info can be found in the doc directory, including schematics and
pictures. Note that the schematic picture is as seen from the back.


Copyright (C) 2020, Óscar Laborda

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
