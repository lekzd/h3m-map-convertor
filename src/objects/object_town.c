
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

#include "../utils.h"

int parse_object_town(json_object *result, struct H3M_OD_ENTRY *od_entry, struct META_OD_ENTRY *meta_od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_TOWN *body
		= (struct H3M_OD_BODY_DYNAMIC_TOWN *)od_entry->body;

	json_object_object_add(result,
			"owner", json_object_new_int(body->owner));

    if (body->has_name) {
		struct H3M_COMMON_STRING *name
			= (struct H3M_COMMON_STRING *)body->name;
        json_object_object_add(result,
			"name", read_string((char *)name, name->size));
    }

	json_object_object_add(result,
			"formation", json_object_new_int(body->formation));

	parse_creatures(result, od_entry, meta_od_entry);

    json_object_object_add(result,
            "has_fort", json_object_new_int(body->has_fort));

    if (body->has_buildings) {
        //TODO: add buildings
    }

	return 0;
}

