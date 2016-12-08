#ifndef __UTILS_H_DEF__
#define __UTILS_H_DEF__

#include <windows.h>

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

int read_string(char *str, int length);

int read_binnary_mask_array(json_object *result, char *attr_name, uint8_t mask[], int length);

#endif
