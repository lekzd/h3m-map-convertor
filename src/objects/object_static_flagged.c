
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int parse_object_static_flagged(json_object *result, struct H3M_OD_ENTRY *od_entry)
{
	struct H3M_OD_BODY_STATIC_FLAGGED *body
		= (struct H3M_OD_BODY_STATIC_FLAGGED *)od_entry->body;

	json_object_object_add(result,
			"owner", json_object_new_int(body->owner));

	return 0;
}

