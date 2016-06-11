
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

static int _parse_hero_secondary_skills(json_object *result, struct H3M_COMMON_SECONDARY_SKILL *skills, unsigned int count)
{
	for (unsigned int i = 0; i < count; ++i) {
		json_object *skill_item;
		skill_item = json_object_new_array();
		json_object_array_add(skill_item, json_object_new_int(skills[i].type));
		json_object_array_add(skill_item, json_object_new_int(skills[i].level));

		json_object_array_add(result, skill_item);
	}

	return 0;
}

static int _add_artifact(json_object *result, char *name, int16_t value)
{
	if (value != -1) {
		json_object_object_add(result, name, json_object_new_int(value));
		return 1;
	}
	return 0;
}

static int _parse_hero_artifacts(json_object *result, union H3M_COMMON_ARTIFACTS *artifacts)
{
	json_object* worn_json;
	worn_json = json_object_new_object();

	_add_artifact(worn_json, "spellbook", artifacts->absod.worn.spellbook);
	_add_artifact(worn_json, "headwear", artifacts->absod.worn.headwear);
	_add_artifact(worn_json, "shoulders", artifacts->absod.worn.shoulders);
	_add_artifact(worn_json, "right_hand", artifacts->absod.worn.right_hand);
	_add_artifact(worn_json, "left_hand", artifacts->absod.worn.left_hand);
	_add_artifact(worn_json, "torso", artifacts->absod.worn.torso);
	_add_artifact(worn_json, "right_ring", artifacts->absod.worn.right_ring);
	_add_artifact(worn_json, "left_ring", artifacts->absod.worn.left_ring);
	_add_artifact(worn_json, "feet", artifacts->absod.worn.feet);
	_add_artifact(worn_json, "misc1", artifacts->absod.worn.misc1);
	_add_artifact(worn_json, "misc2", artifacts->absod.worn.misc2);
	_add_artifact(worn_json, "misc3", artifacts->absod.worn.misc3);
	_add_artifact(worn_json, "misc4", artifacts->absod.worn.misc4);
	_add_artifact(worn_json, "misc5", artifacts->absod.worn.misc5);
	_add_artifact(worn_json, "device1", artifacts->absod.worn.device1);
	_add_artifact(worn_json, "device2", artifacts->absod.worn.device2);
	_add_artifact(worn_json, "device3", artifacts->absod.worn.device3);
	_add_artifact(worn_json, "device4", artifacts->absod.worn.device4);

	json_object_object_add(result, "worn", worn_json);

	json_object* backpack_json;
	int16_t backpack_count = artifacts->sod.backpack.count;
	if (backpack_count != -1) {
		backpack_json = json_object_new_array();
		for (unsigned int i = 0; i < backpack_count; ++i) {
			json_object_array_add(backpack_json, json_object_new_int(artifacts->sod.backpack.artifacts[i]));
		}
		json_object_object_add(result, "backpack", backpack_json);
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
		_parse_hero_secondary_skills(secondary_skills_json, body->secondary_skills->skills, body->secondary_skills->count);
		json_object_object_add(result,
			"secondary_skills", secondary_skills_json);
	}

	if (body->has_artifacts) {
		json_object* artifacts_json;
		artifacts_json = json_object_new_object();
		_parse_hero_artifacts(artifacts_json, body->artifacts);
		json_object_object_add(result,
			"artifacts", artifacts_json);
	}

	json_object_object_add(result,
			"formation", json_object_new_int(body->formation));

	parse_creatures(result, od_entry, meta_od_entry);

	return 0;
}

