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

