/**
 * @file custom_at.cpp
 * @author Bernd Giesecke (bernd@giesecke.tk)
 * @brief
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "app.h"

/** Send Interval offset in flash */
#define SEND_FREQ_OFFSET 0x00000002		 // length 4 bytes
#define BLUES_SETTINGS_OFFSET 0x00000010 // length 518 bytes

// Forward declarations
int interval_send_handler(SERIAL_PORT port, char *cmd, stParam *param);
int status_handler(SERIAL_PORT port, char *cmd, stParam *param);
int buid_handler(SERIAL_PORT port, char *cmd, stParam *param);
int bsim_handler(SERIAL_PORT port, char *cmd, stParam *param);
int breq_handler(SERIAL_PORT port, char *cmd, stParam *param);
int bres_handler(SERIAL_PORT port, char *cmd, stParam *param);

extern uint32_t g_send_repeat_time;

s_blues_settings g_blues_settings;

/**
 * @brief Add send interval AT command
 *
 * @return true if success
 * @return false if failed
 */
bool init_interval_at(void)
{
	return api.system.atMode.add((char *)"SENDINT",
								 (char *)"Set/Get the interval sending time values in seconds 0 = off, max 2,147,483 seconds",
								 (char *)"SENDINT", interval_send_handler,
								 RAK_ATCMD_PERM_WRITE | RAK_ATCMD_PERM_READ);
}

/**
 * @brief Handler for send interval AT commands
 *
 * @param port Serial port used
 * @param cmd char array with the received AT command
 * @param param char array with the received AT command parameters
 * @return int result of command parsing
 * 			AT_OK AT command & parameters valid
 * 			AT_PARAM_ERROR command or parameters invalid
 */
int interval_send_handler(SERIAL_PORT port, char *cmd, stParam *param)
{
	if (param->argc == 1 && !strcmp(param->argv[0], "?"))
	{
		AT_PRINTF("%s=%ld", cmd, g_send_repeat_time / 1000);
	}
	else if (param->argc == 1)
	{
		MYLOG("AT_CMD", "param->argv[0] >> %s", param->argv[0]);
		for (int i = 0; i < strlen(param->argv[0]); i++)
		{
			if (!isdigit(*(param->argv[0] + i)))
			{
				MYLOG("AT_CMD", "%d is no digit", i);
				return AT_PARAM_ERROR;
			}
		}

		uint32_t new_send_freq = strtoul(param->argv[0], NULL, 10);

		MYLOG("AT_CMD", "Requested interval %ld", new_send_freq);

		g_send_repeat_time = new_send_freq * 1000;

		MYLOG("AT_CMD", "New interval %ld", g_send_repeat_time);
		// Stop the timer
		api.system.timer.stop(RAK_TIMER_0);
		if (g_send_repeat_time != 0)
		{
			// Restart the timer
			api.system.timer.start(RAK_TIMER_0, g_send_repeat_time, NULL);
		}
		// Save custom settings
		save_at_setting();
	}
	else
	{
		return AT_PARAM_ERROR;
	}

	return AT_OK;
}

/**
 * @brief Add custom Status AT commands
 *
 * @return true AT commands were added
 * @return false AT commands couldn't be added
 */
bool init_status_at(void)
{
	return api.system.atMode.add((char *)"STATUS",
								 (char *)"Get device information",
								 (char *)"STATUS", status_handler,
								 RAK_ATCMD_PERM_READ);
}

/** Regions as text array */
char *regions_list[] = {"EU433", "CN470", "RU864", "IN865", "EU868", "US915", "AU915", "KR920", "AS923", "AS923-2", "AS923-3", "AS923-4"};
/** Network modes as text array*/
char *nwm_list[] = {"P2P", "LoRaWAN", "FSK"};

/**
 * @brief Print device status over Serial
 *
 * @param port Serial port used
 * @param cmd char array with the received AT command
 * @param param char array with the received AT command parameters
 * @return int result of command parsing
 * 			AT_OK AT command & parameters valid
 * 			AT_PARAM_ERROR command or parameters invalid
 */
int status_handler(SERIAL_PORT port, char *cmd, stParam *param)
{
	String value_str = "";
	int nw_mode = 0;
	int region_set = 0;
	uint8_t key_eui[16] = {0}; // efadff29c77b4829acf71e1a6e76f713

	if (param->argc == 1 && !strcmp(param->argv[0], "?"))
	{
		AT_PRINTF("Device Status:");
		value_str = api.system.hwModel.get();
		value_str.toUpperCase();
		AT_PRINTF("Module: %s", value_str.c_str());
		AT_PRINTF("Version: %s", api.system.firmwareVer.get().c_str());
		AT_PRINTF("Send time: %d s", g_send_repeat_time / 1000);
		nw_mode = api.lorawan.nwm.get();
		AT_PRINTF("Network mode %s", nwm_list[nw_mode]);
		if (nw_mode == 1)
		{
			AT_PRINTF("Network %s", api.lorawan.njs.get() ? "joined" : "not joined");
			region_set = api.lorawan.band.get();
			AT_PRINTF("Region: %d", region_set);
			AT_PRINTF("Region: %s", regions_list[region_set]);
			if (api.lorawan.njm.get())
			{
				AT_PRINTF("OTAA mode");
				api.lorawan.deui.get(key_eui, 8);
				AT_PRINTF("DevEUI = %02X%02X%02X%02X%02X%02X%02X%02X",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7]);
				api.lorawan.appeui.get(key_eui, 8);
				AT_PRINTF("AppEUI = %02X%02X%02X%02X%02X%02X%02X%02X",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7]);
				api.lorawan.appkey.get(key_eui, 16);
				AT_PRINTF("AppKey = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7],
						  key_eui[8], key_eui[9], key_eui[10], key_eui[11],
						  key_eui[12], key_eui[13], key_eui[14], key_eui[15]);
			}
			else
			{
				AT_PRINTF("ABP mode");
				api.lorawan.appskey.get(key_eui, 16);
				AT_PRINTF("AppsKey = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7],
						  key_eui[8], key_eui[9], key_eui[10], key_eui[11],
						  key_eui[12], key_eui[13], key_eui[14], key_eui[15]);
				api.lorawan.nwkskey.get(key_eui, 16);
				AT_PRINTF("NwsKey = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7],
						  key_eui[8], key_eui[9], key_eui[10], key_eui[11],
						  key_eui[12], key_eui[13], key_eui[14], key_eui[15]);
				api.lorawan.daddr.set(key_eui, 4);
				AT_PRINTF("DevAddr = %02X%02X%02X%02X",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3]);
			}
		}
		else if (nw_mode == 0)
		{
			AT_PRINTF("Frequency = %d", api.lorawan.pfreq.get());
			AT_PRINTF("SF = %d", api.lorawan.psf.get());
			AT_PRINTF("BW = %d", api.lorawan.pbw.get());
			AT_PRINTF("CR = %d", api.lorawan.pcr.get());
			AT_PRINTF("Preamble length = %d", api.lorawan.ppl.get());
			AT_PRINTF("TX power = %d", api.lorawan.ptp.get());
		}
		else
		{
			AT_PRINTF("Frequency = %d", api.lorawan.pfreq.get());
			AT_PRINTF("Bitrate = %d", api.lorawan.pbr.get());
			AT_PRINTF("Deviaton = %d", api.lorawan.pfdev.get());
		}
	}
	else
	{
		return AT_PARAM_ERROR;
	}
	return AT_OK;
}

bool init_blues_at(void)
{
	bool result = false;
	result = api.system.atMode.add((char *)"BUID",
								   (char *)"Set/get the Blues product UID",
								   (char *)"BUID", buid_handler,
								   RAK_ATCMD_PERM_WRITE | RAK_ATCMD_PERM_READ);
	result = api.system.atMode.add((char *)"BSIM",
								   (char *)"Set/get Blues SIM settings",
								   (char *)"BSIM", bsim_handler,
								   RAK_ATCMD_PERM_WRITE | RAK_ATCMD_PERM_READ);
	result = api.system.atMode.add((char *)"BREQ",
								   (char *)"Send request to Blues Notecard",
								   (char *)"BREQ", breq_handler,
								   RAK_ATCMD_PERM_WRITE);
	result = api.system.atMode.add((char *)"BRES",
								   (char *)"Factory reset Blues Notecard",
								   (char *)"BRES", bres_handler,
								   RAK_ATCMD_PERM_READ);
	return result;
}

int buid_handler(SERIAL_PORT port, char *cmd, stParam *param)
{
	if (param->argc == 1 && !strcmp(param->argv[0], "?"))
	{
		AT_PRINTF("%s=%s", cmd, g_blues_settings.product_uid);
	}
	else if (param->argc == 2)
	{
		for (int i = 0; param->argv[0][i] != '\0'; i++)
		{
			if (param->argv[0][i] >= 'A' && param->argv[0][i] <= 'Z') // checking for uppercase characters
				param->argv[0][i] = param->argv[0][i] + 32;			  // converting uppercase to lowercase
		}

		for (int i = 0; param->argv[1][i] != '\0'; i++)
		{
			if (param->argv[1][i] >= 'A' && param->argv[1][i] <= 'Z') // checking for uppercase characters
				param->argv[1][i] = param->argv[1][i] + 32;			  // converting uppercase to lowercase
		}

		char new_uid[256] = {0};
		snprintf(new_uid, 255, "%s:%s", param->argv[0], param->argv[1]);

		MYLOG("USR_AT", "Received new Blues Product UID %s", new_uid);

		bool need_save = strcmp(new_uid, g_blues_settings.product_uid) == 0 ? false : true;

		if (need_save)
		{
			snprintf(g_blues_settings.product_uid, 256, new_uid);
		}

		// Save new Blues PUID if changed
		if (need_save)
		{
			save_blues_settings();
		}
		return AT_OK;
	}
	else
	{
		return AT_PARAM_ERROR;
	}
	return AT_OK;
}

int bsim_handler(SERIAL_PORT port, char *cmd, stParam *param)
{
	uint8_t new_sim_usage;
	char new_ext_sim_apn[256];
	bool need_save = false;

	if (param->argc == 1 && !strcmp(param->argv[0], "?"))
	{
		if (g_blues_settings.sim_usage == 0)
		{
			AT_PRINTF("%s=%d", cmd, g_blues_settings.sim_usage);
		}
		else
		{
			AT_PRINTF("%s=%d:%s", cmd, g_blues_settings.sim_usage, g_blues_settings.ext_sim_apn);
		}
	}
	else if (param->argc == 1)
	{
		if (param->argv[0][0] == '0')
		{
			// eSIM only
			MYLOG("USR_AT", "Enable only eSIM");
			new_sim_usage = 0;
		}
		else
		{
			return AT_PARAM_ERROR;
		}
		if (new_sim_usage != g_blues_settings.sim_usage)
		{
			g_blues_settings.sim_usage = new_sim_usage;
			need_save = true;
		}
	}
	else if (param->argc == 2)
	{
		if (param->argv[0][0] == '1')
		{
			// External SIM only
			MYLOG("USR_AT", "Enable only external SIM");
			new_sim_usage = 1;
		}
		else if (param->argv[0][0] == '2')
		{
			// External SIM only
			MYLOG("USR_AT", "Primary external SIM, secondary eSIM");
			new_sim_usage = 2;
		}
		else if (param->argv[0][0] == '3')
		{
			// prim ESIM, sec external SIM
			MYLOG("USR_AT", "Primary eSIM, secondary external SIM");
			new_sim_usage = 3;
		}
		else
		{
			return AT_PARAM_ERROR;
		}
		for (int i = 0; param->argv[1][i] != '\0'; i++)
		{
			if (param->argv[1][i] >= 'A' && param->argv[1][i] <= 'Z') // checking for uppercase characters
				param->argv[1][i] = param->argv[1][i] + 32;			  // converting uppercase to lowercase
		}
		snprintf(new_ext_sim_apn, 256, "%s", param->argv[1]);

		if (new_sim_usage != g_blues_settings.sim_usage)
		{
			g_blues_settings.sim_usage = new_sim_usage;
			need_save = true;
		}
		if (strcmp(new_ext_sim_apn, g_blues_settings.product_uid) != 0)
		{
			snprintf(g_blues_settings.ext_sim_apn, 256, new_ext_sim_apn);
			need_save = true;
		}
	}
	else
	{
		return AT_PARAM_ERROR;
	}
	if (need_save)
	{
		save_blues_settings();
	}
	return AT_OK;
}

int breq_handler(SERIAL_PORT port, char *cmd, stParam *param)
{
	bool request_success = false;
	if (param->argc == 1)
	{
		for (int try_send = 0; try_send < 3; try_send++)
		{
			if (rak_blues.start_req(param->argv[0]))
			{
				if (rak_blues.send_req(rsp_msg, 1024))
				{
					AT_PRINTF(">>%s<<", rsp_msg);
					request_success = true;
					break;
				}
			}
			delay(100);
		}
		if (!request_success)
		{
			MYLOG("BLUES", "hub.set request failed");
			return AT_ERROR;
		}
		return AT_OK;
	}
	else
	{
		return AT_PARAM_ERROR;
	}
	return AT_OK;
}

int bres_handler(SERIAL_PORT port, char *cmd, stParam *param)
{
	if (param->argc == 0)
	{
		blues_card_restore();
		return AT_OK;
	}
	else
	{
		return AT_PARAM_ERROR;
	}
	return AT_OK;
}

/**
 * @brief Get setting from flash
 *
 * @return false read from flash failed or invalid settings type
 */
bool get_at_setting(void)
{
	uint8_t flash_value[16];
	if (!api.system.flash.get(SEND_FREQ_OFFSET, flash_value, 5))
	{
		MYLOG("AT_CMD", "Failed to read send interval from Flash");
		return false;
	}
	if (flash_value[4] != 0xAA)
	{
		MYLOG("AT_CMD", "No valid send interval found, set to default, read 0X%02X 0X%02X 0X%02X 0X%02X",
			  flash_value[0], flash_value[1],
			  flash_value[2], flash_value[3]);
		g_send_repeat_time = 0;
		save_at_setting();
		return false;
	}
	MYLOG("AT_CMD", "Read send interval 0X%02X 0X%02X 0X%02X 0X%02X",
		  flash_value[0], flash_value[1],
		  flash_value[2], flash_value[3]);
	g_send_repeat_time = 0;
	g_send_repeat_time |= flash_value[0] << 0;
	g_send_repeat_time |= flash_value[1] << 8;
	g_send_repeat_time |= flash_value[2] << 16;
	g_send_repeat_time |= flash_value[3] << 24;
	MYLOG("AT_CMD", "Send interval found %ld", g_send_repeat_time);
	return true;
}

/**
 * @brief Save setting to flash
 *
 * @return true write to flash was successful
 * @return false write to flash failed or invalid settings type
 */
bool save_at_setting(void)
{
	uint8_t flash_value[16] = {0};
	bool wr_result = false;
	flash_value[0] = (uint8_t)(g_send_repeat_time >> 0);
	flash_value[1] = (uint8_t)(g_send_repeat_time >> 8);
	flash_value[2] = (uint8_t)(g_send_repeat_time >> 16);
	flash_value[3] = (uint8_t)(g_send_repeat_time >> 24);
	flash_value[4] = 0xAA;
	MYLOG("AT_CMD", "Writing send interval 0X%02X 0X%02X 0X%02X 0X%02X ",
		  flash_value[0], flash_value[1],
		  flash_value[2], flash_value[3]);
	wr_result = api.system.flash.set(SEND_FREQ_OFFSET, flash_value, 5);
	if (!wr_result)
	{
		// Retry
		wr_result = api.system.flash.set(SEND_FREQ_OFFSET, flash_value, 5);
	}
	wr_result = true;
	return wr_result;
}

bool read_blues_settings(void)
{
	uint8_t *blues_settings = (uint8_t *)&g_blues_settings;
	if (api.system.flash.get(BLUES_SETTINGS_OFFSET, blues_settings, 518))
	{
		if (g_blues_settings.valid_mark == 0xAA55)
		{
			return true;
		}
		else
		{
			// No settings, use defaults
			g_blues_settings.valid_mark = 0xAA55;											  // Validity marker
			snprintf(g_blues_settings.product_uid, 256, "com.my-company.my-name:my-project"); // Standard UID
			g_blues_settings.conn_continous = false;										  // Use periodic connection
			g_blues_settings.sim_usage = 0;													  // 0 int SIM, 1 ext SIM, 2 ext int SIM, 3 int ext SIM
			snprintf(g_blues_settings.ext_sim_apn, 256, "internet");						  // APN
			g_blues_settings.motion_trigger = false;										  // Send data on motion trigger (unused in this app)
		}
	}
	MYLOG("USR_AT", "No Blues parameters found");
	return false;
}

bool save_blues_settings(void)
{
	g_blues_settings.valid_mark != 0xAA55;
	uint8_t *blues_settings = (uint8_t *)&g_blues_settings;
	if (api.system.flash.set(BLUES_SETTINGS_OFFSET, blues_settings, 518))
	{
		return true;
	}
	MYLOG("USR_AT", "Saving Blues parameters failed");
	return false;
}