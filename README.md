# WisBlock Goes Blues
| <img src="./assets/RAK-Whirls.png" alt="RAKWireless"> | <img src="./assets/rakstar.jpg" alt="RAKstar" > | <img src="./assets/blues_logo.jpg" alt="Blues" width="200%" height="200%" style="background-color:white"> |    
| :-: | :-: | :-: | 

This library covers the communication between a [RAKwireless WisCore module](https://docs.rakwireless.com/Product-Categories/WisBlock/#wisblock-core)↗️ and a [Blues.IO Notecard](https://blues.io/notecard-cellular/)↗️ over I2C.     

Blues offers a library with a wide functionality for the communication between a MCU and the Notecard, supporting different communication methods.    

I decided to write a lightweight library just for the RAK13102 WisBlock Notecarrier IO module with the following differences:    
- The RAK13102 module supports only I2C communication. No need for the overhead to support multiple interfaces
- This library works with static memory allocation and the ArduinoJson library to create the notes instead of dynamic memory allocation of the original Blues library
- Using a single JSON object for both the request and the response to and from the NoteCard

----

# Installation    
The library is available in the Arduino library manager and in the PlatformIO library registry. 

In the Arduino IDE the installation is done through the Arduino Library Manager using the keyword _**`blues-minimal-i2c`**_.    

In PlatformIO the installation is done by adding 
```log
lib_deps = 
	beegee-tokyo/Blues-Minimal-I2C
```
in the platformio.ini file.

----

# API calls

The API calls are split into 3 categories:
- Initialization and execution
   - Initialize class, start a request, send a request to the Notecard
- Set request JSON object
   - Add JSON objects to the Notecard request
- Parse response JSON object
   - Get JSON objects from the Notecard response

----

## Initialization and execution

----

#### `public  `[`RAK_BLUES`](#class_r_a_k___b_l_u_e_s_1a4b8e60752a63a5cc3d10d3363a0546db)`(byte addr)` 

Construct a new [RAK_BLUES](#class_r_a_k___b_l_u_e_s) instance.

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"hub.set"))
    {
        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

#### `public bool `[`start_req`](#class_r_a_k___b_l_u_e_s_1a974467279b8494189c5e7571ef0bb77e)`(char * request)` 

Create a request structure to be sent to the NoteCard.

#### Parameters
* `request` name of request, e.g. card.wireless 

#### Returns
true if request could be created 

#### Returns
false if request could not be created 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"hub.set"))
    {
        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

#### `public bool `[`send_req`](#class_r_a_k___b_l_u_e_s_1ae47c82e40288c97ba2653063056b68fa)`(char * response,uint16_t resp_len)` 

Send a completed request to the NoteCard.

#### Parameters
* `response` if not NULL, the response of the Notecard will copied into this buffer 

* `resp_len` max length of buffer for the Notecard response 

#### Returns
true if request could be sent and the response does not have "err" 

#### Returns
false if request could not be sent or the response did have "err" 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"hub.set"))
    {
        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

## Set request JSON object

----

#### `public void `[`add_string_entry`](#class_r_a_k___b_l_u_e_s_1a791f19b0ec5ecaf939614b88674e3b70)`(char * type,char * value)` 

Add C-String entry to request.

#### Parameters
* `type` char * name 

* `value` char * value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"hub.set"))
    {
        rak_blues.add_string_entry((char *)"mode", (char *)"continuous");
        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

#### `public void `[`add_bool_entry`](#class_r_a_k___b_l_u_e_s_1afa0f02b0b0fa76dc5d809d8cf5b5e516)`(char * type,bool value)` 

Add boolean entry to request.

#### Parameters
* `type` char * name 

* `value` bool value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"card.motion.mode"))
    {
        rak_blues.add_bool_entry((char *)"start", true);
        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

#### `public void `[`add_int32_entry`](#class_r_a_k___b_l_u_e_s_1a9960f72ecb1cbdb35e7103471d9c5f51)`(char * type,int32_t value)` 

Add integer entry to request.

#### Parameters
* `type` char * name 

* `value` integer value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"card.motion.mode"))
    {
        rak_blues.add_int32_entry((char *)"sensitivity", 1);

        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

#### `public void `[`add_uint32_entry`](#class_r_a_k___b_l_u_e_s_1a14d1718ca743a3450ca53bb206601453)`(char * type,uint32_t value)` 

Add unsigned integer entry to request.

#### Parameters
* `type` char * name 

* `value` unsigned integer value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"card.motion.mode"))
    {
        rak_blues.add_uint32_entry((char *)"sensitivity", 1);

        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

#### `public void `[`add_float_entry`](#class_r_a_k___b_l_u_e_s_1af19973b3bc6e56642e049f2994f9ab73)`(char * type,float value)` 

Add float entry to request.

#### Parameters
* `type` char * name 

* `value` float value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

void setup()
{
    if (rak_blues.start_req((char *)"card.motion.mode"))
    {
        rak_blues.add_float_entry((char *)"sensitivity", 324.56);

        if (rak_blues.send_req())
        {
            request_success = true;
            break;
        }
    }
}

void loop()
{
}
```

----

#### `public void `[`add_nested_string_entry`](#class_r_a_k___b_l_u_e_s_1a82f72528723042c8686b12f81302a0ea)`(char * type,char * nested,char * value)` 

Add nested C-String entry to request.

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `value` char * value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        char node_id[24];
        sprintf(node_id, "%02x%02x%02x%02x%02x%02x%02x%02x",
            g_lorawan_settings.node_device_eui[0], g_lorawan_settings.node_device_eui[1],
            g_lorawan_settings.node_device_eui[2], g_lorawan_settings.node_device_eui[3],
            g_lorawan_settings.node_device_eui[4], g_lorawan_settings.node_device_eui[5],
            g_lorawan_settings.node_device_eui[6], g_lorawan_settings.node_device_eui[7]);
        rak_blues.add_nested_string_entry((char *)"body", (char *)"dev_eui", node_id);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`add_nested_int32_entry`](#class_r_a_k___b_l_u_e_s_1a3d9147f5260ee7d29cdcafb834ba9d66)`(char * type,char * nested,int32_t value)` 

Add nested integer entry to request.

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `value` integer value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_int32_entry((char *)"body", (char *)"number", -65534);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`add_nested_uint32_entry`](#class_r_a_k___b_l_u_e_s_1ae800da06402c47dc1ecc2032827271a5)`(char * type,char * nested,uint32_t value)` 

Add nested unsigned integer entry to request.

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `value` unsigned integer value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_uint32_entry((char *)"body", (char *)"number", 65534);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`add_nested_bool_entry`](#class_r_a_k___b_l_u_e_s_1a5dd813b397bfdb837698c6078d8ce291)`(char * type,char * nested,bool value)` 

Add nested bool entry to request.

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `value` char * value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_bool_entry((char *)"body", (char *)"valid", false);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`add_nested_float_entry`](#class_r_a_k___b_l_u_e_s_1aa7a85cac6bd6618e0020a92e6e2115fd)`(char * type,char * nested,float value)` 

Add nested float entry to request.

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `value` float value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_float_entry((char *)"body", (char *)"temperature", 32.8);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`add_2lv_nested_string_entry`](#class_r_a_k___b_l_u_e_s_1a82f72528723042c8686b12f81302a0ea)`(char * type,char * nested,char * nested2,char * value)` 

Add 2 level nested C-String entry to request

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `nested2` char * 2nd level nested name 

* `value` char * value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        char node_id[24];
        sprintf(node_id, "%02x%02x%02x%02x%02x%02x%02x%02x",
            g_lorawan_settings.node_device_eui[0], g_lorawan_settings.node_device_eui[1],
            g_lorawan_settings.node_device_eui[2], g_lorawan_settings.node_device_eui[3],
            g_lorawan_settings.node_device_eui[4], g_lorawan_settings.node_device_eui[5],
            g_lorawan_settings.node_device_eui[6], g_lorawan_settings.node_device_eui[7]);
        rak_blues.add_nested_string_entry((char *)"body", (char *)"sens1", (char *)"dev_eui", node_id);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`add_2lv_nested_int32_entry`](#class_r_a_k___b_l_u_e_s_1a3d9147f5260ee7d29cdcafb834ba9d66)`(char * type,char * nested,char * nested2,int32_t value)` 

Add 2 level nested integer entry to request

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `nested2` char * 2nd level nested name 

* `value` integer value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_int32_entry((char *)"body", (char *)"sens1", (char *)"number", -65534);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`add_2lv_nested_uint32_entry`](#class_r_a_k___b_l_u_e_s_1ae800da06402c47dc1ecc2032827271a5)`(char * type,char * nested,char * nested2,uint32_t value)` 

Add nested unsigned integer entry to request.

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `nested2` char * 2nd level nested name 

* `value` unsigned integer value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_uint32_entry((char *)"body", (char *)"sens1", (char *)"number", 65534);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public void `[`Add 2 level nested bool entry to request`](#class_r_a_k___b_l_u_e_s_1a5dd813b397bfdb837698c6078d8ce291)`(char * type,char * nested,char * nested2,bool value)` 

Add 2 level nested bool entry to request

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `nested2` char * 2nd level nested name 

* `value` bool value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_bool_entry((char *)"body", (char *)"sens1", (char *)"valid", false);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_2lv_nested_string_entry`](#class_r_a_k___b_l_u_e_s_1aca9ab69b08a7b43d22c5c6166438878b)`(char * type,char * nested,char * nested2,char * value,uint16_t value_size)` 

Get 2nd level nested string entry (char array) from the response

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `nested2` nested 2nd level name as char * 

* `value` (out) address of char array to write the string to 

* `value_size` size of target char array 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

char str_value[128];

void setup()
{
	if (rak_blues.start_req((char *)"card.location"))
	{
		if (!rak_blues.send_req())
		{
			return false;
		}
		if (rak_blues.has_nested_entry((char *)"status",(char *)"sens_1"))
		{
			rak_blues.get_2lv_nested_string_entry((char *)"status",(char *)"sens_1",(char *)"error", str_value, 128);
		}
		return true;
	}
}

  void loop()
  {
  }
```

----

#### `public bool `[`get_2lv_nested_int32_entry`](#class_r_a_k___b_l_u_e_s_1a58e88df4c7f7229479a7856ba17f822f)`(char * type,char * nested,char * nested2,int32_t & value)` 

Get 2nd level nested signed 32bit integer entry from the response

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `nested2` nested 2nd level name as char * 

* `value` (out) address of signed 32bit integer variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

int32_t value;

void setup()
{
	if (rak_blues.start_req((char *)"card.location"))
	{
		if (!rak_blues.send_req())
		{
			return false;
		}
		if (rak_blues.has_nested_entry((char *)"status",(char *)"sens_1"))
		{
			rak_blues.get_nested_int32_entry((char *)"status",(char *)"sens_1",(char *)"value", value);
		}
		return true;
	}
}

  void loop()
  {
  }
```

----

#### `public bool `[`get_2lv_nested_uint32_entry`](#class_r_a_k___b_l_u_e_s_1a5348eeaeddab010538def5912acc06dd)`(char * type,char * nested,char * nested2,uint32_t & value)` 

Get 2nd level nested unsigned 32bit integer entry from the response

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `nested2` nested 2nd level name as char * 

* `value` (out) address of unsigned 32bit integer variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 uint32_t value;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_nested_entry((char *)"status",(char *)"sens_1"))
        {
            rak_blues.get_2lv_nested_uint32_entry((char *)"status", (char *)"sens1", (char *)"value", value);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_2lv_nested_bool_entry`](#class_r_a_k___b_l_u_e_s_1a95fd867e024a5215de9c9f2f25649d4b)`(char * type,char * nested,char * nested2,bool & value)` 

Get nested bool entry from the response.

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `nested2` nested 2nd level name as char * 

* `value` (out) address of bool variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

bool value;

void setup()
{
	if (rak_blues.start_req((char *)"card.location"))
	{
		if (!rak_blues.send_req())
		{
			return false;
		}
		if (rak_blues.has_nested_entry((char *)"status",(char *)"sens_1"))
		{
			rak_blues.get_2lv_nested_bool_entry((char *)"status",(char *)"sens_1"),(char *)"value"), value);
		}
		return true;
	}
}

void loop()
{
}
```

----

#### `public void `[`add_2lv_nested_float_entry`](#class_r_a_k___b_l_u_e_s_1aa7a85cac6bd6618e0020a92e6e2115fd)`(char * type,char * nested,char * nested2,float value)` 

Add nested float entry to request.

#### Parameters
* `type` char * name 

* `nested` char * nested name 

* `nested2` char * 2nd level nested name 

* `value` float value 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 void setup()
 {
    if (rak_blues.start_req((char *)"note.add"))
    {
        rak_blues.add_nested_float_entry((char *)"body", (char *)"sens1", (char *)"temperature", 32.8);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
 }

void loop()
{
}
```

----

## Parse response JSON object

----

#### `public bool `[`has_entry`](#class_r_a_k___b_l_u_e_s_1afde6572e783efc4bb4aa65cc7dd27d5d)`(char * type)` 

Check if the response has a specific entry.

#### Parameters
* `type` entry name as char * 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 char str_value[128];

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_entry((char *)"status"))
        {
            rak_blues.get_string_entry((char *)"status", str_value, 128);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`has_nested_entry`](#class_r_a_k___b_l_u_e_s_1af402cc8eebd74342e74df30c90a9d793)`(char * type,char * nested)` 

Check if the response has a specific nested entry.

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 char str_value[128];

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_nested_entry((char *)"status"), (char *)"error"))
        {
            rak_blues.get_nested_string_entry((char *)"status", (char *)"error"), str_value, 128);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_string_entry`](#class_r_a_k___b_l_u_e_s_1af087168c9313e6998f27ab8df3452b65)`(char * type,char * value,uint16_t value_size)` 

Get string entry (char array) from the response.

#### Parameters
* `type` entry name as char * 

* `value` (out) address of char array to write the string to 

* `value_size` size of target char array 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 char str_value[128];

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_entry((char *)"status"))
        {
            rak_blues.get_string_entry((char *)"status", str_value, 128);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_string_entry_from_array`](#class_r_a_k___b_l_u_e_s_1ae563ae697aa40dcca1441318bf7622a6)`(char * type,char * value,uint16_t value_size)` 

Get string entry (char array) from response when in an array.

#### Parameters
* `type` entry name as char * 

* `value` (out) address of char array to write the string to 

* `value_size` size of target char array 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <[blues-minimal-i2c.h](#blues-minimal-i2c_8h)>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 char str_value[128];

 // {"files":["motion"],"set":true}

 void setup()
 {
    if (rak_blues.start_req((char *)"card.attn"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_entry((char *)"files"))
        {
            rak_blues.get_string_entry((char *)"files", str_value, 128);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_bool_entry`](#class_r_a_k___b_l_u_e_s_1a76daa94ddda7d6319d0b7463cfbde68e)`(char * type,bool & value)` 

Get bool entry from the response.

#### Parameters
* `type` entry name as char * 

* `value` (out) address of bool variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 bool result;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_entry((char *)"status"))
        {
            rak_blues.get_bool_entry((char *)"status", result);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_int32_entry`](#class_r_a_k___b_l_u_e_s_1aa29d7a21c9f0c62c6436246134239b29)`(char * type,int32_t & value)` 

Get signed 32bit integer entry from the response.

#### Parameters
* `type` entry name as char * 

* `value` (out) address of signed integer variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 int32_t value;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_entry((char *)"status"))
        {
            rak_blues.get_int32_entry((char *)"status", value);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_uint32_entry`](#class_r_a_k___b_l_u_e_s_1adc0ae319d15f36e9d2578ebe33274d96)`(char * type,uint32_t & value)` 

Get unsigned 32bit integer entry from the response.

#### Parameters
* `type` entry name as char * 

* `value` (out) address of unsigned integer variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 uint32_t value;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_entry((char *)"status"))
        {
            rak_blues.get_uint32_entry((char *)"status", value);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_float_entry`](#class_r_a_k___b_l_u_e_s_1a21c448ac36ebf6c42fd6dc7dbe4797d0)`(char * type,float & value)` 

Get float entry from the response.

#### Parameters
* `type` entry name as char * 

* `value` (out) address of float variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 float blues_latitude;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_entry((char *)"lat"))
        {
            rak_blues.get_float_entry((char *)"lat", blues_latitude);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_nested_string_entry`](#class_r_a_k___b_l_u_e_s_1aca9ab69b08a7b43d22c5c6166438878b)`(char * type,char * nested,char * value,uint16_t value_size)` 

Get nested string entry (char array) from the response.

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `value` (out) address of char array to write the string to 

* `value_size` size of target char array 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 char str_value[128];

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_nested_entry((char *)"status"),(char *)"error"))
        {
            rak_blues.get_nested_string_entry((char *)"status"),(char *)"error"), str_value, 128);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_nested_int32_entry`](#class_r_a_k___b_l_u_e_s_1a58e88df4c7f7229479a7856ba17f822f)`(char * type,char * nested,int32_t & value)` 

Get nested signed 32bit integer entry from the response.

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `value` (out) address of signed 32bit integer variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 int32_t value;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_nested_entry((char *)"status"),(char *)"value"))
        {
            rak_blues.get_nested_int32_entry((char *)"status"),(char *)"value"), value);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_nested_uint32_entry`](#class_r_a_k___b_l_u_e_s_1a5348eeaeddab010538def5912acc06dd)`(char * type,char * nested,uint32_t & value)` 

Get nested unsigned 32bit integer entry from the response.

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `value` (out) address of unsigned 32bit integer variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 uint32_t value;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_nested_entry((char *)"status"),(char *)"value"))
        {
            rak_blues.get_nested_uint32_entry((char *)"status"),(char *)"value"), value);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public bool `[`get_nested_bool_entry`](#class_r_a_k___b_l_u_e_s_1a95fd867e024a5215de9c9f2f25649d4b)`(char * type,char * nested,bool & value)` 

Get nested bool entry from the response.

#### Parameters
* `type` entry name as char * 

* `nested` nested level name as char * 

* `value` (out) address of bool variable to write the value to 

#### Returns
true if entry was found 

#### Returns
false if entry was not found 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 bool value;

 void setup()
 {
    if (rak_blues.start_req((char *)"card.location"))
    {
        if (!rak_blues.send_req())
        {
            return false;
        }
        if (rak_blues.has_nested_entry((char *)"status"),(char *)"value"))
        {
            rak_blues.get_nested_bool_entry((char *)"status"),(char *)"value"), value);
        }
        return true;
    }
 }

void loop()
{
}
```

----

#### `public int `[`myJB64Encode`](#class_r_a_k___b_l_u_e_s_1a85bb573ff6e5da8bf0a1615023da1b69)`(char * encoded,const char * string,int len)` 

Encode a char buffer to Base64.

#### Parameters
* `encoded` (out) encoded string 

* `string` char buffer for encoding 

* `len` length of buffer 

#### Returns
int length of encoded string 

Example
```cpp
#include <Arduino.h>
#include <blues-minimal-i2c.h>
// I2C functions for Blues NoteCard
RAK_BLUES rak_blues;

 char data[] = {0x00, 0x01, 0x02, 0x03};
 int data_len = 4;
 char payload_b86[255];

 void setup()
 {

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

        rak_blues.myJB64Encode(payload_b86, (const char *)data, data_len);

        rak_blues.add_string_entry((char *)"payload", payload_b86);

        if (!rak_blues.send_req())
        {
            return false;
        }
        return true;
    }
    return false;
 }

void loop()
{
}
```

----

