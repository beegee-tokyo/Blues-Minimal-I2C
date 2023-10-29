/**
 * @file blues.cpp
 * @author Bernd Giesecke (bernd@giesecke.tk)
 * @brief Blues.IO NoteCard handler
 * @version 0.1
 * @date 2023-04-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "main.h"
#include <blues-minimal-i2c.h>

// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

#ifndef PRODUCT_UID
#define PRODUCT_UID "com.my-company.my-name:my-project"
#endif
#define myProductID PRODUCT_UID

/** Flag if GNSS is in continuous or periodic mode */
bool gnss_continuous = true;

/**
 * @brief Initialize Blues NoteCard
 *
 * @return true if NoteCard was found and setup was successful
 * @return false if NoteCard was not found or the setup failed
 */
bool init_blues(void)
{
	Wire.begin();
	Wire.setClock(100000);
	// Get the ProductUID from the saved settings
	// If no settings are found, use NoteCard internal settings!
	if (read_blues_settings())
	{
		bool request_success = false;

		MYLOG("BLUES", "Found saved settings, override NoteCard internal settings!");
		if (memcmp(g_blues_settings.product_uid, "com.my-company.my-name", 22) == 0)
		{
			MYLOG("BLUES", "No Product ID saved");
			AT_PRINTF(":EVT NO PUID");
			memcpy(g_blues_settings.product_uid, PRODUCT_UID, 33);
		}

		MYLOG("BLUES", "Set Product ID and connection mode");
		for (int try_send = 0; try_send < 3; try_send++)
		{
			if (rak_blues.start_req((char *)"hub.set"))
			{
				rak_blues.add_string_entry((char *)"product", g_blues_settings.product_uid);
				if (g_blues_settings.conn_continous)
				{
					rak_blues.add_string_entry((char *)"mode", (char *)"continuous");
				}
				else
				{
					rak_blues.add_string_entry((char *)"mode", (char *)"minimum");
				}
				// // Set sync time to 20 times the sensor read time
				// add_int32_entry((char *)"seconds", (g_lorawan_settings.send_repeat_time * 20 / 1000));
				// add_bool_entry((char *)"heartbeat", true);

				if (rak_blues.send_req())
				{
					request_success = true;
					break;
				}
			}
			delay(100);
		}
		if (!request_success)
		{
			MYLOG("BLUES", "hub.set request failed");
			return false;
		}
		request_success = false;

		MYLOG("BLUES", "Set SIM and APN");
		for (int try_send = 0; try_send < 3; try_send++)
		{
			if (rak_blues.start_req((char *)"card.wireless"))
			{
				rak_blues.add_string_entry((char *)"mode", (char *)"auto");

				switch (g_blues_settings.sim_usage)
				{
				case 0:
					// USING BLUES eSIM CARD
					rak_blues.add_string_entry((char *)"method", (char *)"primary");
					break;
				case 1:
					// USING EXTERNAL SIM CARD only
					rak_blues.add_string_entry((char *)"apn", g_blues_settings.ext_sim_apn);
					rak_blues.add_string_entry((char *)"method", (char *)"secondary");
					break;
				case 2:
					// USING EXTERNAL SIM CARD as primary
					rak_blues.add_string_entry((char *)"apn", g_blues_settings.ext_sim_apn);
					rak_blues.add_string_entry((char *)"method", (char *)"dual-secondary-primary");
					break;
				case 3:
					// USING EXTERNAL SIM CARD as secondary
					rak_blues.add_string_entry((char *)"apn", g_blues_settings.ext_sim_apn);
					rak_blues.add_string_entry((char *)"method", (char *)"dual-primary-secondary");
					break;
				}

				if (rak_blues.send_req())
				{
					request_success = true;
					break;
				}
			}
		}
		if (!request_success)
		{
			MYLOG("BLUES", "card.wireless request failed");
			return false;
		}
		request_success = false;
	}

	// Disable location (just in case)
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.location.mode"))
		{
			rak_blues.add_string_entry((char *)"mode", (char *)"off");
			if (rak_blues.send_req())
			{
				break;
			}
		}
	}

	// Disable location tracking (just in case)
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.location.track"))
		{
			rak_blues.add_bool_entry((char *)"stop", true);
			if (rak_blues.send_req())
			{
				break;
			}
		}
	}

	// Disable motion mode (just in case)
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.motion.mode"))
		{
			rak_blues.add_bool_entry((char *)"stop", true);
			if (rak_blues.send_req())
			{
				break;
			}
		}
	}

	// Disable motion sync (just in case)
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.motion.sync"))
		{
			rak_blues.add_bool_entry((char *)"stop", true);
			if (rak_blues.send_req())
			{
				break;
			}
		}
	}

	// Disable motion tracking (just in case)
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.motion.track"))
		{
			rak_blues.add_bool_entry((char *)"stop", true);
			if (rak_blues.send_req())
			{
				break;
			}
		}
	}

	// Get card version
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.version"))
		{
			if (rak_blues.send_req())
			{
				break;
			}
		}
	}
	return true;
}

/**
 * @brief Send a data packet to NoteHub.IO
 *
 * @param data Payload as byte array (CayenneLPP formatted)
 * @param data_len Length of payload
 * @return true if note could be sent to NoteCard
 * @return false if note send failed
 */
bool blues_send_payload(uint8_t *data, uint16_t data_len)
{
	char payload_b86[255];

	if (rak_blues.start_req((char *)"note.add"))
	{
		rak_blues.add_string_entry((char *)"file", (char *)"data.qo");
		rak_blues.add_bool_entry((char *)"sync", true);
		char node_id[24];
		sprintf(node_id, "%02x%02x%02x%02x%02x%02x%02x%02x",
				g_lorawan_settings.node_device_eui[0], g_lorawan_settings.node_device_eui[1],
				g_lorawan_settings.node_device_eui[2], g_lorawan_settings.node_device_eui[3],
				g_lorawan_settings.node_device_eui[4], g_lorawan_settings.node_device_eui[5],
				g_lorawan_settings.node_device_eui[6], g_lorawan_settings.node_device_eui[7]);
		rak_blues.add_nested_string_entry((char *)"body", (char *)"dev_eui", node_id);
		float bme680[3];
		get_rak1906_values(bme680);
		rak_blues.add_nested_float_entry((char *)"body", (char *)"temp", bme680[0]);
		rak_blues.add_nested_float_entry((char *)"body", (char *)"humid", bme680[1]);
		rak_blues.add_nested_float_entry((char *)"body", (char *)"baro", bme680[2]);

		rak_blues.myJB64Encode(payload_b86, (const char *)data, data_len);

		rak_blues.add_string_entry((char *)"payload", payload_b86);

		MYLOG("BLUES", "Finished parsing");
		if (!rak_blues.send_req())
		{
			MYLOG("BLUES", "Send request failed");
			return false;
		}
		AT_PRINTF("+EVT:CELLULAR_SENT");
		return true;
	}
	return false;
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
		if (rak_blues.send_req())
		{
			request_success = true;
			break;
		}
	}
	if (!request_success)
	{
		MYLOG("BLUES", "hub.status request failed");
	}
}

/**
 * @brief Reset the Notecard to factory settings
 *
 */
void blues_card_restore(void)
{
	rak_blues.start_req((char *)"hub.status");
	rak_blues.add_bool_entry((char *)"delete", true);
	rak_blues.add_bool_entry((char *)"connected", true);
	rak_blues.send_req();
}
