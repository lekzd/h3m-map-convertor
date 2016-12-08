#include <windows.h>

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

json_object* read_string(char *str, int length)
{
	json_object *result;
	result = json_object_new_array();
	for (unsigned int i = 0; i < length; ++i) {
		uint8_t charCode = (uint8_t)str[i];
		if (charCode != 0) {
			json_object_array_add(result, json_object_new_int(charCode));
		}
	}

	return result;
}

int read_binnary_mask_array(json_object *result, char *attr_name, uint8_t mask[], int length)
{
	json_object *values_array;
	values_array = json_object_new_array();
	int empty = 1;
	for (unsigned int i = 0; i < length; ++i) {
		uint8_t charCode = (uint8_t)mask[i];
		json_object_array_add(values_array, json_object_new_int(charCode));
		if (charCode) {
			empty = 0;
		}
	}
	if (empty) {
		json_object_object_add(result, attr_name, json_object_new_array());
	}
	else {
		json_object_object_add(result, attr_name, values_array);
	}
	return 0;
}
