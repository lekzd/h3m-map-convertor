
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int parse_object_town(json_object *result, struct H3M_OD_ENTRY *od_entry, struct META_OD_ENTRY *meta_od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_TOWN *body
		= (struct H3M_OD_BODY_DYNAMIC_TOWN *)od_entry->body;

	json_object_object_add(result,
			"owner", json_object_new_int(body->owner));

    //TODO correct reading of multibyte name
    if (body->has_name) {
        json_object_object_add(result,
            "name", json_object_new_string((char *)body->name));
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

