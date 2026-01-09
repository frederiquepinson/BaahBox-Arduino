// ******************************************
// * Baah Box Arduino : Sensor BTLE gateway *
// ******************************************

// Copyright (C) 2017 – 2025 Orange SA

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef genericSensor_hpp
#define genericSensor_hpp

#include "Config/BBConfig.hpp"
#include "../BBScheduler.hpp"
#include <Wire.h>
#include <Adafruit_ADS7830.h>

class genericSensorClass
{

public:
  genericSensorClass(void);
  ~genericSensorClass();

  int init(unsigned long period);
  int memory[NB_ANALOG_SENSORS] = {};
  void getAnalogInputs(int *input1, int *input2);
  int sensorAcquisition(char *bleMessage);
  int I2CAcquisition(char *bleMessage, Adafruit_ADS7830 ad7830);
  int storedValues[NB_ANALOG_SENSORS];
  BBScheduler *scheduler;

private:
  int lowpass(int value, int index);
};

#endif /* muscleSensor_hpp */
