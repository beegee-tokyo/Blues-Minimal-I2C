/**
 * @file app.h
 * @author Bernd Giesecke (bernd@giesecke.tk)
 * @brief Defines, includes, global definitions
 * @version 0.1
 * @date 2023-05-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include "wisblock_cayenne.h"

/** Payload buffer */
extern WisCayenne g_solution_data;

/** Time interval to send packets in milliseconds */
extern uint32_t g_send_repeat_time;

// Debug
// Debug output set to 0 to disable app debug output
#ifndef MY_DEBUG
#define MY_DEBUG 1
#endif

#if MY_DEBUG > 0
#define MYLOG(tag, ...)                  \
	do                                   \
	{                                    \
		if (tag)                         \
			Serial.printf("[%s] ", tag); \
		Serial.printf(__VA_ARGS__);      \
		Serial.printf("\n");             \
	} while (0);                         \
	delay(100)
#else
#define MYLOG(...)
#endif

#if defined(_VARIANT_RAK3172_) || defined(_VARIANT_RAK3172_SIP_)
#define AT_PRINTF(...)              \
	do                              \
	{                               \
		Serial.printf(__VA_ARGS__); \
		Serial.printf("\r\n");      \
	} while (0);                    \
	delay(100)
#else // RAK4630 || RAK11720
#define AT_PRINTF(...)               \
	do                               \
	{                                \
		Serial.printf(__VA_ARGS__);  \
		Serial.printf("\r\n");       \
		Serial6.printf(__VA_ARGS__); \
		Serial6.printf("\r\n");      \
	} while (0);                     \
	delay(100)
#endif

// Forward declarations
void send_packet(void);
bool init_status_at(void);
bool init_interval_at(void);
bool get_at_setting(void);
bool save_at_setting(void);
bool read_blues_settings(void);
bool save_blues_settings(void);

// Cayenne LPP Channel numbers per sensor value
#define LPP_CHANNEL_BATT 1			   // Base Board
#define LPP_CHANNEL_HUMID 2			   // RAK1901
#define LPP_CHANNEL_TEMP 3			   // RAK1901
#define LPP_CHANNEL_PRESS 4			   // RAK1902
#define LPP_CHANNEL_LIGHT 5			   // RAK1903
#define LPP_CHANNEL_HUMID_2 6		   // RAK1906
#define LPP_CHANNEL_TEMP_2 7		   // RAK1906
#define LPP_CHANNEL_PRESS_2 8		   // RAK1906
#define LPP_CHANNEL_GAS_2 9			   // RAK1906
#define LPP_CHANNEL_GPS 10			   // RAK1910/RAK12500
#define LPP_CHANNEL_SOIL_TEMP 11	   // RAK12035
#define LPP_CHANNEL_SOIL_HUMID 12	   // RAK12035
#define LPP_CHANNEL_SOIL_HUMID_RAW 13  // RAK12035
#define LPP_CHANNEL_SOIL_VALID 14	   // RAK12035
#define LPP_CHANNEL_LIGHT2 15		   // RAK12010
#define LPP_CHANNEL_VOC 16			   // RAK12047
#define LPP_CHANNEL_GAS 17			   // RAK12004
#define LPP_CHANNEL_GAS_PERC 18		   // RAK12004
#define LPP_CHANNEL_CO2 19			   // RAK12008
#define LPP_CHANNEL_CO2_PERC 20		   // RAK12008
#define LPP_CHANNEL_ALC 21			   // RAK12009
#define LPP_CHANNEL_ALC_PERC 22		   // RAK12009
#define LPP_CHANNEL_TOF 23			   // RAK12014
#define LPP_CHANNEL_TOF_VALID 24	   // RAK12014
#define LPP_CHANNEL_GYRO 25			   // RAK12025
#define LPP_CHANNEL_GESTURE 26		   // RAK14008
#define LPP_CHANNEL_UVI 27			   // RAK12019
#define LPP_CHANNEL_UVS 28			   // RAK12019
#define LPP_CHANNEL_CURRENT_CURRENT 29 // RAK16000
#define LPP_CHANNEL_CURRENT_VOLTAGE 30 // RAK16000
#define LPP_CHANNEL_CURRENT_POWER 31   // RAK16000
#define LPP_CHANNEL_TOUCH_1 32		   // RAK14002
#define LPP_CHANNEL_TOUCH_2 33		   // RAK14002
#define LPP_CHANNEL_TOUCH_3 34		   // RAK14002
#define LPP_CHANNEL_CO2_2 35		   // RAK12037
#define LPP_CHANNEL_CO2_Temp_2 36	   // RAK12037
#define LPP_CHANNEL_CO2_HUMID_2 37	   // RAK12037
#define LPP_CHANNEL_TEMP_3 38		   // RAK12003
#define LPP_CHANNEL_TEMP_4 39		   // RAK12003


/** Module stuff */
bool init_rak1906(void);
void start_rak1906(void);
bool read_rak1906(void);
extern bool has_rak1906;
extern float bme680_temp;
extern float bme680_humidity;
extern float bme680_barometer;

// Blues.io
struct s_blues_settings
{
	uint16_t valid_mark = 0xAA55;								 // Validity marker
	char product_uid[256] = "com.my-company.my-name:my-project"; // Blues Product UID
	bool conn_continous = false;								 // Use periodic connection
	uint8_t sim_usage = 0;										 // 0 int SIM, 1 ext SIM, 2 ext int SIM, 3 int ext SIM
	char ext_sim_apn[256] = "internet";							 // APN to be used with external SIM
	bool motion_trigger = true;									 // Send data on motion trigger
};

#include <blues-minimal-i2c.h>

bool init_blues_at(void);
bool init_blues(void);
bool blues_send_payload(uint8_t *data, uint16_t data_len);
void blues_card_restore(void);
extern RAK_BLUES rak_blues;
extern s_blues_settings g_blues_settings;
extern bool has_blues;
extern char rsp_msg[];
