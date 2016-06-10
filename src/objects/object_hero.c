
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

static int _parse_hero_primary_skills(json_object *result, struct H3M_COMMON_PRIMARY_SKILLS *primary_skills)
{
	json_object_object_add(result,
		"attack", json_object_new_int(primary_skills->attack_skill));
	json_object_object_add(result,
		"defense", json_object_new_int(primary_skills->defense_skill));
	json_object_object_add(result,
		"power", json_object_new_int(primary_skills->spell_power));
	json_object_object_add(result,
		"knowledge", json_object_new_int(primary_skills->knowledge));

	return 0;
}

static int _parse_hero_secondary_skills(json_object *result, struct H3M_AI_HERO_SETTINGS_SECONDARY_SKILLS *secondary_skills)
{

	for (unsigned int i = 0; i <= secondary_skills->count - 1; ++i) {
		json_object *skill_item;
		skill_item = json_object_new_array();	
		//json_object_array_add(skill_item, json_object_new_int(secondary_skills->entries[i].type));
		//json_object_array_add(skill_item, json_object_new_int(secondary_skills->entries[i].level));

		json_object_array_add(result, skill_item);
	}

	return 0;
}

int parse_object_hero(json_object *result, struct H3M_OD_ENTRY *od_entry, struct META_OD_ENTRY *meta_od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_HERO *body 
		= (struct H3M_OD_BODY_DYNAMIC_HERO *)od_entry->body;

	if (body->has_name) {
		json_object_object_add(result,
			"name", json_object_new_string((char *)body->name));
	}

	if (body->has_biography) {
		json_object_object_add(result,
			"biography", json_object_new_string((char *)body->biography));
	}

	json_object_object_add(result,
			"owner", json_object_new_int(body->owner));

	json_object_object_add(result,
			"person", json_object_new_int(body->type));

	json_object_object_add(result,
			"gender", json_object_new_int(body->gender));

	if (body->has_face) {
		json_object_object_add(result,
			"portrait", json_object_new_int(body->face));
	}

	if (body->has_experience) {
		json_object_object_add(result,
			"experience", json_object_new_int(body->experience));
	}

	if (body->has_primary_skills) {
		json_object* primary_skills_json;
		primary_skills_json = json_object_new_object();
		_parse_hero_primary_skills(primary_skills_json, body->primary_skills);
		json_object_object_add(result,
			"primary_skills", primary_skills_json);
	}

	if (body->has_secondary_skills) {
		json_object* secondary_skills_json;
		secondary_skills_json = json_object_new_array();
		_parse_hero_secondary_skills(secondary_skills_json, body->secondary_skills);
		json_object_object_add(result,
			"secondary_skills", secondary_skills_json);
	}

	json_object_object_add(result,
			"formation", json_object_new_int(body->formation));

	parse_creatures(result, od_entry, meta_od_entry);

	return 0;
}

