
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

static int _parse_slots(union H3M_COMMON_ARMY *army, json_object *result)
{
	for (unsigned int i = 0; i <= 6; ++i) {
		json_object *creature_stack_item;
		creature_stack_item = json_object_new_array();
		if (army->absod.slots[i].type != 65535) {
			json_object_array_add(creature_stack_item, json_object_new_int(army->absod.slots[i].type));
			json_object_array_add(creature_stack_item, json_object_new_int(army->absod.slots[i].quantity));
		}
		json_object_array_add(result, creature_stack_item);
	}
	return 0;
}

static int _parse_hero_creatures(json_object *result, struct H3M_OD_ENTRY *od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_HERO *body
		= (struct H3M_OD_BODY_DYNAMIC_HERO *)od_entry->body;

	if (body->has_creatures) {
		_parse_slots(body->creatures, result);
		return 1;
	}
	return 0;
}

static int _parse_town_creatures(json_object *result, struct H3M_OD_ENTRY *od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_TOWN *body
		= (struct H3M_OD_BODY_DYNAMIC_TOWN *)od_entry->body;

	if (body->has_creatures) {
		_parse_slots(body->creatures, result);
		return 1;
	}
	return 0;
}

int parse_creatures(json_object *result, struct H3M_OD_ENTRY *od_entry, struct META_OD_ENTRY *meta_od_entry)
{
	json_object *creatures_json;
	unsigned int has_creatures = 0;
	creatures_json = json_object_new_array();

	switch (meta_od_entry->oa_type) {

		case H3M_OBJECT_HERO:
		case H3M_OBJECT_RANDOM_HERO:
		case H3M_OBJECT_PRISON:
			has_creatures = _parse_hero_creatures(creatures_json, od_entry);
			break;

		case H3M_OBJECT_TOWN:
		case H3M_OBJECT_TOWN_ABSOD:
			has_creatures = _parse_town_creatures(creatures_json, od_entry);
			break;

	}

	if (has_creatures) {
		json_object_object_add(result,
			"creatures", creatures_json);
	}
    
    return 0;
}

