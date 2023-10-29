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
#include "app.h"

// I2C functions for Blues NoteCard
RAK_BLUES rak_blues(0x17);

#ifndef PRODUCT_UID
#define PRODUCT_UID "com.my-company.my-name:my-project"
#endif
#define myProductID PRODUCT_UID

/** Char buffer for card response */
char rsp_msg[1024];

/** Flag if the Blues Notecard is available */
bool has_blues = false;

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

	/*******************************************************************************/
	/** Reset all location and motion modes to non-active, just in case            */
	/*******************************************************************************/
	// Disable location (just in case)
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.location.mode"))
		{
			rak_blues.add_string_entry((char *)"mode", (char *)"off");
			if (rak_blues.send_req(rsp_msg, 1024))
			{
				MYLOG("BLUES", "card.location.mode: %s", rsp_msg);
				break;
			}
		}
		else
		{
			MYLOG("BLUES", "Could not create request for card.location.mode");
		}
	}

	// Disable location tracking (just in case)
	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.location.track"))
		{
			rak_blues.add_bool_entry((char *)"stop", true);
			if (rak_blues.send_req(rsp_msg, 1024))
			{
				MYLOG("BLUES", "card.location.track: %s", rsp_msg);
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
			if (rak_blues.send_req(rsp_msg, 1024))
			{
				MYLOG("BLUES", "card.motion.mode: %s", rsp_msg);
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
			if (rak_blues.send_req(rsp_msg, 1024))
			{
				MYLOG("BLUES", "card.motion.sync: %s", rsp_msg);
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
			if (rak_blues.send_req(rsp_msg, 1024))
			{
				MYLOG("BLUES", "card.motion.track: %s", rsp_msg);
				break;
			}
		}
	}

	bool request_success = false;

	// Get the ProductUID from the saved settings
	// If no settings are found, use NoteCard internal settings!
	if (read_blues_settings())
	{

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
				// Set sync time to the sensor read time
				rak_blues.add_int32_entry((char *)"seconds", (g_send_repeat_time / 1000));
				rak_blues.add_bool_entry((char *)"heartbeat", true);

				if (rak_blues.send_req(rsp_msg, 1024))
				{
					request_success = true;
					MYLOG("BLUES", "hub.set: %s", rsp_msg);
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

				if (rak_blues.send_req(rsp_msg, 1024))
				{
					request_success = true;
					MYLOG("BLUES", "card.wireless: %s", rsp_msg);
					break;
				}
			}
		}
		if (!request_success)
		{
			MYLOG("BLUES", "card.wireless request failed");
			return false;
		}
	}
	request_success = false;

	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"card.version"))
		{
			if (rak_blues.send_req(rsp_msg, 1024))
			{
				MYLOG("BLUES", "card.version: %s", rsp_msg);
				request_success = true;
				break;
			}
			else
			{
				MYLOG("BLUES", "Send request failed");
			}
		}
		else
		{
			MYLOG("BLUES", "Start request failed");
		}
	}

	return request_success;
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
	bool request_success = false;

	for (int try_send = 0; try_send < 3; try_send++)
	{
		if (rak_blues.start_req((char *)"note.add"))
		{
			rak_blues.add_string_entry((char *)"file", (char *)"data.qo");
			rak_blues.add_bool_entry((char *)"sync", true);
			char node_id[24];
			uint8_t node_device_eui[8];
			api.lorawan.deui.get(node_device_eui, 8);
			sprintf(node_id, "%02x%02x%02x%02x%02x%02x%02x%02x",
					node_device_eui[0], node_device_eui[1],
					node_device_eui[2], node_device_eui[3],
					node_device_eui[4], node_device_eui[5],
					node_device_eui[6], node_device_eui[7]);
			rak_blues.add_nested_string_entry((char *)"body", (char *)"dev_eui", node_id);
			rak_blues.add_nested_float_entry((char *)"body", (char *)"temp", bme680_temp);
			rak_blues.add_nested_float_entry((char *)"body", (char *)"humid", bme680_humidity);
			rak_blues.add_nested_float_entry((char *)"body", (char *)"baro", bme680_barometer);

			rak_blues.myJB64Encode(payload_b86, (const char *)data, data_len);

			rak_blues.add_string_entry((char *)"payload", payload_b86);

			MYLOG("BLUES", "Payload prepared");

			if (rak_blues.send_req())
			{
				request_success = true;
				break;
			}
		}
	}
	if (!request_success)
	{
		MYLOG("BLUES", "Send request failed");
		return false;
	}

	return request_success;
}

/**
 * @brief Reset Blues Notecard to factory settings
 *
 */
void blues_card_restore(void)
{
	rak_blues.start_req((char *)"hub.status");
	rak_blues.add_bool_entry((char *)"delete", true);
	rak_blues.add_bool_entry((char *)"connected", true);
	rak_blues.send_req();
}
