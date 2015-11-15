
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int parse_object_random_dwelling(json_object *result, struct H3M_OD_ENTRY *od_entry, struct META_OD_ENTRY *meta_od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_RANDOM_DWELLING *body 
		= (struct H3M_OD_BODY_DYNAMIC_RANDOM_DWELLING *)od_entry->body;

	json_object_object_add(result,
			"owner", json_object_new_int(body->owner));

    if (H3M_OBJECT_RANDOM_DWELLING_PRESET_ALIGNMENT_ABSOD != meta_od_entry->oa_type) {

        json_object_object_add(result,
            "castle_id", json_object_new_int(body->castle_absod_id));

        json_object_object_add(result,
            "alignment", json_object_new_string(body->alignment));
    }

    if (H3M_OBJECT_RANDOM_DWELLING_PRESET_LEVEL_ABSOD != meta_od_entry->oa_type) {
        json_object_object_add(result,
            "min_level", json_object_new_int(body->min_level));

        json_object_object_add(result,
            "max_level", json_object_new_int(body->max_level));
    }


	return 0;
}

