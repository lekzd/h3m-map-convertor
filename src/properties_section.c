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
	result = h3m->bi.any.has_two_levels;
	return 0;
}

static int _get_difficulty_level(int *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	result = h3m->bi.any.difficulty;
	return 0;
}

static int _get_map_name(WCHAR *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	strcpy(result, h3m->bi.any.name);
	return 0;
}

static int _get_map_desc(WCHAR *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	strcpy(result, h3m->bi.any.desc);
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
	_get_has_caves(hasCaves, ctx);
	json_object_object_add(result, 
		"has_caves", json_object_new_int(hasCaves));

	int difficulty = 0;
	_get_difficulty_level(difficulty, ctx);
	json_object_object_add(result, 
		"difficulty", json_object_new_int(difficulty));

	WCHAR name[255] = L"";
	_get_map_name(name, ctx);
	json_object_object_add(result, 
		"name", json_object_new_string(name));

	WCHAR desc[255] = L"";
	_get_map_desc(desc, ctx);
	json_object_object_add(result, 
		"descr", json_object_new_string(desc));

	return 0;
}
