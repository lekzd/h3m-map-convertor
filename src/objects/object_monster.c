
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int _parse_monster_treasure(json_object *result, struct H3M_OD_BODY_EXT_MESG_AND_TREASURE body)
{
	if (body.mesg_size) {
		json_object_object_add(result,
			"message", json_object_new_string(body.mesg));
	}
	
	struct H3M_OD_BODY_EXT_TREASURE_ABSOD *treasure
		= (struct H3M_OD_BODY_EXT_TREASURE_ABSOD *)body.treasure;

	if (treasure->artifact != 65535) {
		json_object_object_add(result,
			"artifact", json_object_new_int(treasure->artifact));
	}

	if (treasure->resources.resource) {
		json_object* resources_json;
		resources_json = json_object_new_array();
		for (unsigned int i = 0; i < 7; i++) {
			json_object_array_add(resources_json, json_object_new_int(treasure->resources.resource[i]));
		}
		json_object_object_add(result,
			"resources", resources_json);
	}
	return 0;
}

int parse_object_monster(json_object *result, struct H3M_OD_ENTRY *od_entry, struct META_OD_ENTRY *meta_od_entry)
{
	struct H3M_OD_BODY_STATIC_MONSTER *body
		= (struct H3M_OD_BODY_STATIC_MONSTER *)od_entry->body;

	struct H3M_OD_BODY_DYNAMIC_MONSTER *dynamic_body
		= (struct H3M_OD_BODY_DYNAMIC_MONSTER *)od_entry->body;

	if (body->quantity) {
		json_object_object_add(result,
			"quantity", json_object_new_int(body->quantity));
	}

	json_object_object_add(result,
		"disposition", json_object_new_int(body->disposition));

	if ((uint8_t)body->does_not_grow) {
		json_object_object_add(result,
			"does_not_grow", json_object_new_boolean(1));
	}

	if ((uint8_t)body->never_flees) {
		json_object_object_add(result,
			"never_flees", json_object_new_boolean(1));
	}

	if (body->has_mesg_and_treasure) {
		_parse_monster_treasure(result, dynamic_body->mesg_and_treasure);
	}

	return 0;
}

