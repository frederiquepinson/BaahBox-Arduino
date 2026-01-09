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

#include <Arduino.h>

#ifdef USE_NRF51
#include "BLE/Nrf51/btle.hpp"
#include "Display/ADA_OLED_FEATHERWING/display.hpp"
#endif

#ifdef USE_NRF52
#include "BLE/Nrf52/btle.hpp"
#include "Display/ADA_OLED_FEATHERWING/display.hpp"
#endif

#ifdef USE_ESP32S3
#include "BLE/ESP32S3/btle.hpp"
#include "Display/ADA_ESP32_TFT/display.hpp"
#include <Wire.h>
#include <Adafruit_ADS7830.h>
#endif

#include "Sensors/genericSensor.hpp"
#include "Config/BBConfig.hpp"

btleClass btle;
genericSensorClass genericSensor;
BBDisplay bbDisplay;
BBConfigClass config;
char sensorData[10];
#ifdef USE_ESP32S3
Adafruit_ADS7830 ad7830;
#endif

//*********************************************
//*
//*       S E T U P
//*
//*********************************************
void setup()
{
#ifdef __DEBUG__
    {
        // initialize serial communication
        Serial.begin(38400);
        int timeoutInitSerial = 100;
        while (timeoutInitSerial-- > 0)
        {
            if (Serial)
                break;
            delay(10);
        }
        Serial.println("Serial initialized");
    }
#endif
    // load parameters
    config.init();
    // initialize BTLE
    char tmpDeviceName[50];
    config.btleDeviceName.toCharArray(tmpDeviceName, 50);
    btle.init(tmpDeviceName);
    Serial.print("BTLE initialized => ");
    Serial.println(tmpDeviceName);

#ifdef USE_ESP32S3
    // initialize I2C broker
    if (!ad7830.begin())
    {
        Serial.println("Failed to initialize ADS7830!");
        while (1)
            ;
    }
    Serial.println(" AD7830 OK!");
#endif

    // initialize Sensors

    genericSensor.init(SENSOR_ACQUISITION_PERIOD_IN_MS);
    Serial.println("Sensors initialized");

    // initialize Display
    bbDisplay.init();

    Serial.println("end of main setup");
}

//*********************************************
//*
//*       L O O P
//*
//*********************************************
void loop()
{
    bbDisplay.checkButtons();
    if (genericSensor.scheduler->needToBeExecuted())
    {
#ifdef USE_ESP32S3
        int length = genericSensor.I2CAcquisition(sensorData, ad7830);
#else
        int length = genericSensor.sensorAcquisition(sensorData);
#endif
        btle.write(sensorData, length);
    }
    if (bbDisplay.scheduler->needToBeExecuted())
    {
        bbDisplay.update();
    }
    delay(MAIN_LOOP_DELAY);
}
