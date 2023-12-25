/**
 * @file main.cpp
 * @author Bernd Giesecke (bernd@giesecke.tk)
 * @brief Minimal sketch to send data over RAK13102 Blues Notecard
 *
 * Requires setup of Blues Notecard through https://dev.blues.io/quickstart/blues-quickstart/
 * The code only reports whether the Blues NoteCard has cellular connection and sends a
 * data packet every 2 minutes.
 * Uses library Blues-Minimal-I2C library
 *
 * @version 0.1
 * @date 2023-12-08
 *
 * @copyright Copyright (c) 2023
 *
 */

/*********************************************************/
/*********************************************************/
/*                                                       */
/* REQUIRED LIBRARIES                                    */
/*                                                       */
// http://librarymanager/All#SX126x-Arduino
// http://librarymanager/All#Blues-Minimal-I2C
/** Only needed for RAK11200 ESP32                       */
// http://librarymanager/All#NimBLE-Arduino
/*********************************************************/
/*********************************************************/

#include <Arduino.h>
#include <SPI.h>
#ifdef NRF52_SERIES
#include <Adafruit_TinyUSB.h>
#endif
#include <blues-minimal-i2c.h>
#include <SX126x-Arduino.h>

// Forward declarations
bool blues_hub_connected(void);
void blues_hub_status(void);

// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

bool blues_connected = false;

char blues_response[2048];

/**
 * @brief Arduino setup function
 *
 */
void setup(void)
{
	Serial.begin(115200);
	time_t serial_timeout = millis();
	// On nRF52840 the USB serial is not available immediately
	while (!Serial)
	{
		if ((millis() - serial_timeout) < 5000)
		{
			delay(100);
			digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
		}
		else
		{
			break;
		}
	}

	// Initialize LoRa transceiver and send to sleep
#ifdef NRF52_SERIES
	lora_rak4630_init();
#endif
#ifdef ESP32
	lora_rak13300_init();
#endif
	Radio.Sleep();

	// Initialize Blues Notecard interface
	Wire.begin();
	Wire.setClock(100000);
}

/**
 * @brief Arduino loop function
 *
 */
void loop()
{
	Serial.println("======================================================");
	// Check if Blues Notecard has cellular connection
	blues_connected = blues_hub_connected();
	if (blues_connected)
	{
		Serial.println("NoteCard reports it has cellular connection");
	}
	else
	{
		Serial.println("NoteCard reports it has no cellular connection");
	}

	Serial.println("======================================================");
	// Prepare a simple data packet
	if (rak_blues.start_req((char *)"note.add"))
	{
		// Create data file
		rak_blues.add_string_entry((char *)"file", (char *)"data.qo");
		// Force sync
		rak_blues.add_bool_entry((char *)"sync", true);
		// Add some float values
		rak_blues.add_nested_float_entry((char *)"body", (char *)"temp", 36.76);
		rak_blues.add_nested_float_entry((char *)"body", (char *)"humid", 67);
		rak_blues.add_nested_float_entry((char *)"body", (char *)"baro", 1024);

		// Add connction status
		if (blues_connected)
		{
			rak_blues.add_nested_string_entry((char *)"body", (char *)"status", (char *)"connected");
		}
		else
		{
			rak_blues.add_nested_string_entry((char *)"body", (char *)"status", (char *)"not connected");
		}

		Serial.println("Payload created");
		if (!rak_blues.send_req())
		{
			Serial.println("Send request failed : 'note.add'");
		}
		else
		{
			Serial.println("Request sent to NoteCard");
		}
	}
	else
	{
		Serial.println("Error creating the 'note.add' request");
	}

	Serial.println("======================================================");

	// Get NoteCard status
	blues_hub_status();

	delay(60000);
	Serial.println("\n");
}

/**
 * @brief Check connection to cellular network
 *
 * @verbose
 *
 * @return true if connection is/was established
 * @return false if no connection
 */
bool blues_hub_connected(void)
{
	bool request_success = false;
	bool cellular_connected = false;
	for (int try_send = 0; try_send < 5; try_send++)
	{
		// Prepare "card.wireless" request
		if (rak_blues.start_req((char *)"card.wireless"))
		{
			// Send request
			if (rak_blues.send_req())
			{
				// Check if the response has an entry "net"
				if (rak_blues.has_entry((char *)"net"))
				{
					// Check if the response has an entry "band"
					if (rak_blues.has_nested_entry((char *)"net", (char *)"band"))
					{
						// If "band" is available, the NoteCard should have cellular connection
						cellular_connected = true;
					}
					request_success = true;
					break;
				}
			}
		}
	}
	if (!request_success)
	{
		Serial.println("'card.wireless' request failed");
		return false;
	}
	return cellular_connected;
}

/**
 * @brief Request NoteHub status, only for debug purposes
 *
 */
void blues_hub_status(void)
{
	bool request_success = false;
	for (int try_send = 0; try_send < 3; try_send++)
	{
		rak_blues.start_req((char *)"hub.status");
		if (rak_blues.send_req(blues_response, 2048))
		{
			Serial.println("Notecard Hub status:");
			Serial.printf("%s\n", blues_response);
			request_success = true;
			break;
		}
	}
	if (!request_success)
	{
		Serial.println("'hub.status' request failed");
	}
	Serial.println("======================================================");

	request_success = false;
	for (int try_send = 0; try_send < 3; try_send++)
	{
		rak_blues.start_req((char *)"hub.get");
		if (rak_blues.send_req(blues_response, 2048))
		{
			Serial.println("Notecard Hub setup:");
			Serial.printf("%s\n", blues_response);
			request_success = true;
			break;
		}
	}
	if (!request_success)
	{
		Serial.println("'hub.get' request failed");
	}
	Serial.println("======================================================");

	request_success = false;
	for (int try_send = 0; try_send < 3; try_send++)
	{
		rak_blues.start_req((char *)"card.wireless");
		if (rak_blues.send_req(blues_response, 2048))
		{
			Serial.println("Notecard wireless status:");
			Serial.printf("%s\n", blues_response);
			request_success = true;
			break;
		}
	}
	if (!request_success)
	{
		Serial.println("'card.wireless' request failed");
	}
	Serial.println("======================================================");
}
