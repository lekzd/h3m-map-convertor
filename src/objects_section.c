#include "../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

#include "./objects/object_hero.h"

static int _get_bytes_array(json_object *result, char *bytes)
{
	int count = 6;
	int i = 0;
	char *strcat(char *dest, const char *src);
	for (i = 0; i < count; ++i) {
		uint8_t byte = bytes[i];
		json_object_array_add(result,
			json_object_new_int(byte));
	}
	return 0;
}

static int _get_entity_coords_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	struct H3M_OD_ENTRY *entry = NULL;
	int i = 0;

	for (i = 0; i < h3m->od.count; ++i) {
		entry = &h3m->od.entries[i];
		if (entry->header.oa_index == id) {

			json_object_array_add(result,
				json_object_new_int(entry->header.x));

			json_object_array_add(result,
				json_object_new_int(entry->header.y));

			json_object_array_add(result,
				json_object_new_int(entry->header.z));

			break;
		}
	}
	return 0;
}

static int _get_entity_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	struct H3MLIB_META *meta = &((struct H3MLIB_CTX *)ctx)->meta;
	struct META_OD_ENTRY *meta_od_entry = NULL;
	struct H3M_OD_ENTRY *od_entry = NULL;
	int i = 0;

	for (i = 0; i < h3m->od.count; ++i) {
		meta_od_entry = &meta->od_entries[i];
		od_entry = &h3m->od.entries[i];

		if (od_entry->header.oa_index != id) {
			continue;
		}
		switch (meta_od_entry->oa_type) {
			case H3M_OBJECT_DWELLING:
		    case H3M_OBJECT_DWELLING_ABSOD:
		    	json_object_object_add(result, "type", json_object_new_string("dwelling"));
		    case H3M_OBJECT_LIGHTHOUSE:
		    	json_object_object_add(result, "type", json_object_new_string("lighthouse"));
		    case H3M_OBJECT_RESOURCE_GENERATOR:
		    	json_object_object_add(result, "type", json_object_new_string("mill"));
		    case H3M_OBJECT_SHIPYARD:
		    	json_object_object_add(result, "type", json_object_new_string("shipyard"));
		    case H3M_OBJECT_ABANDONED_MINE_ABSOD:
		    	json_object_object_add(result, "type", json_object_new_string("abandoned_mine"));
		    	parse_object_static_flagged(result, od_entry);
				break;

			case H3M_OBJECT_HERO:
				json_object_object_add(result, "type", json_object_new_string("hero"));
				parse_object_hero(result, od_entry);
				break;
		}
		break;
	}
	return 0;
}

static int _get_object_json_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	struct H3M_OA_ENTRY *entry = NULL;
	entry = &h3m->oa.entries[id];

	json_object_object_add(result, 
		"id", json_object_new_int(id));

	json_object_object_add(result, 
		"texture", json_object_new_string((char *)entry->header.def));

	json_object *entity_coords_data;
	entity_coords_data = json_object_new_array();
	_get_entity_coords_data(entity_coords_data, ctx, id);
	json_object_object_add(result, 
		"coords", entity_coords_data);

	json_object *passability_json;
	passability_json = json_object_new_array();
	_get_bytes_array(passability_json, (char *)entry->body.passable);
	json_object_object_add(result, 
		"passability", passability_json);

	json_object *actions_json;
	actions_json = json_object_new_array();
	_get_bytes_array(actions_json, (char *)entry->body.active);
	json_object_object_add(result, 
		"actions", actions_json);

	json_object *entity_data;
	entity_data = json_object_new_object();
	_get_entity_data(entity_data, ctx, id);
	json_object_object_add(result, 
		"data", entity_data);

	json_object_object_add(result, 
		"object_class",	json_object_new_int((int *)entry->body.object_class));

	return 0;
}

int get_map_objects_json(json_object *result, h3mlib_ctx_t ctx)
{
	json_object *object;
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	int i = 0;

	for (i = 0; i < h3m->oa.count; ++i) {
		object = json_object_new_object();
		_get_object_json_data(object, ctx, i);
		json_object_array_add(result, object);
	}
	return 0;
}
