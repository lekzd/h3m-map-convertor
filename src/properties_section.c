#include <windows.h>

#include "../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

static int _get_map_version(char *result, h3mlib_ctx_t ctx)
{
	switch (h3m_get_format(ctx)) {
	case H3M_FORMAT_ROE: {
		strcpy(result, "RoE");
		break;
	}
	case H3M_FORMAT_AB: {
		strcpy(result, "AB");
		break;
	}
	case H3M_FORMAT_SOD: {
		strcpy(result, "SoD");
		break;
	}
	case H3M_FORMAT_WOG: {
		strcpy(result, "WoG");
		break;
	}
	}
	return 0;
}

static int _get_has_caves(int *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	*result = (int)h3m->bi.any.has_two_levels;
	return 0;
}

static int _get_difficulty_level(int *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	*result = h3m->bi.any.difficulty;
	return 0;
}

static int _get_map_name(char *result, size_t n, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	if (h3m->bi.any.name != NULL) {
		strncpy(result, (char *)h3m->bi.any.name, n);
	} else {
		*result = '\0';
	}
	return 0;
}

static int _get_map_desc(char *result, size_t n, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	if (h3m->bi.any.name != NULL) {
		strncpy(result, (char *)h3m->bi.any.desc, n);
	} else {
		*result = '\0';
	}
	return 0;
}

static int _get_win_condition(json_object *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	json_object_object_add(result,
		"type", json_object_new_int(h3m->ai.any.win_cond_type));
	json_object_object_add(result,
		"allow_normal_win", json_object_new_int(h3m->ai.any.win_cond.c_flag.allow_normal_win));
	json_object_object_add(result,
		"applies_to_computer", json_object_new_int(h3m->ai.any.win_cond.c_flag.applies_to_computer));
	return 0;
}

static int _get_lose_condition(json_object *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	json_object_object_add(result,
		"type", json_object_new_int(h3m->ai.any.lose_cond_type));
	return 0;
}

int get_map_properties(json_object *result, h3mlib_ctx_t ctx)
{
	char mapVersion[4];
	_get_map_version(mapVersion, ctx);
	json_object_object_add(result, 
		"version", json_object_new_string(mapVersion));

	size_t mapSize = h3m_get_map_size(ctx);
	json_object_object_add(result, 
		"map_size", json_object_new_int(mapSize));

	int hasCaves = 0;
	_get_has_caves(&hasCaves, ctx);
	json_object_object_add(result, 
		"has_caves", json_object_new_int(hasCaves));

	int difficulty = 0;
	_get_difficulty_level(&difficulty, ctx);
	json_object_object_add(result, 
		"difficulty", json_object_new_int(difficulty));

	char name[255] = { 0 };
	_get_map_name(name, sizeof(name), ctx);
	json_object_object_add(result, 
		"name", json_object_new_string(name));

	char desc[255] = { 0 };
	_get_map_desc(desc, sizeof(desc), ctx);
	json_object_object_add(result, 
		"descr", json_object_new_string(desc));

	json_object *win_conditions_json;
	win_conditions_json = json_object_new_object();
	_get_win_condition(win_conditions_json, ctx);
	json_object_object_add(result,
		"win_condition", win_conditions_json);

	json_object *lose_conditions_json;
	lose_conditions_json = json_object_new_object();
	_get_lose_condition(lose_conditions_json, ctx);
	json_object_object_add(result,
		"lose_condition", lose_conditions_json);

	return 0;
}
