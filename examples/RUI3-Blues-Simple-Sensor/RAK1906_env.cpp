/**
 * @file RAK1906_env.cpp
 * @author Bernd Giesecke (bernd@giesecke.tk)
 * @brief BME680 sensor functions
 * @version 0.1
 * @date 2022-04-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "app.h"

#include "rak1906.h" // Install manually from https://downloads.rakwireless.com/RUI/RUI3/Library/ ==> RAKwireless_RAK1906_Environment_BME680.zip

/** BME680 instance for Wire */
rak1906 bme;

/** Temperature */
float bme680_temp = 0.0;
/** Humidity */
float bme680_humidity = 0.0;
/** Barometric pressure */
float bme680_barometer = 0.0;

/** Flag if the sensor is available */
bool has_rak1906 = false;

/**
 * @brief Initialize the BME680 sensor
 *
 * @return true if sensor was found
 * @return false if sensor was not found
 */
bool init_rak1906(void)
{
	Wire.begin();

	if (!bme.init())
	{
		// MYLOG("BME", "Could not find a valid BME680 sensor, check wiring!");
		return false;
	}
	// MYLOG("BME", "BME680 sensor ok");
	return true;
}

/**
 * @brief Read environment data from BME680
 *     Data is added to Cayenne LPP payload as channels
 *     LPP_CHANNEL_HUMID_2, LPP_CHANNEL_TEMP_2,
 *     LPP_CHANNEL_PRESS_2 and LPP_CHANNEL_GAS_2
 *
 *
 * @return true if reading was successful
 * @return false if reading failed
 */
bool read_rak1906()
{
	// MYLOG("BME", "Reading BME680");

	bme680_temp = 0.0;
	bme680_humidity = 0.0;
	bme680_barometer = 0.0;

	if (bme.update())
	{
		bme680_temp = bme.temperature();
		bme680_humidity = bme.humidity();
		bme680_barometer = bme.pressure();
		MYLOG("BME", "Temperature: %.2f", bme680_temp);
		MYLOG("BME", "Humidity: %.2f", bme680_humidity);
		MYLOG("BME", "Barometric pressure: %.2f", bme680_barometer);
		g_solution_data.addRelativeHumidity(LPP_CHANNEL_HUMID, bme680_humidity);
		g_solution_data.addTemperature(LPP_CHANNEL_TEMP, bme680_temp);
		g_solution_data.addBarometricPressure(LPP_CHANNEL_PRESS, bme680_barometer);
		// g_solution_data.addAnalogInput(LPP_CHANNEL_GAS, (float)(bme.gas()) / 1000.0);

		return true;
	}
	else
	{
		MYLOG("BME", "Failed to read BME");
		return false;
	}
}
