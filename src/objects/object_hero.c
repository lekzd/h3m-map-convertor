
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int parse_object_hero(json_object *result, struct H3M_OD_ENTRY *od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_HERO *body 
		= (struct H3M_OD_BODY_DYNAMIC_HERO *)od_entry->body;

	if (body->has_name) {
		json_object_object_add(result,
			"name", json_object_new_string((char *)body->name));
	}

	json_object_object_add(result,
			"owner", json_object_new_int(body->owner));

	if (body->has_face) {
		json_object_object_add(result,
			"portrait", json_object_new_int(body->face));
	}

	if (body->has_experience) {
		json_object_object_add(result,
			"experience", json_object_new_int(body->experience));
	}

	json_object_object_add(result,
			"gender", json_object_new_boolean(body->gender));

	return 0;
}

