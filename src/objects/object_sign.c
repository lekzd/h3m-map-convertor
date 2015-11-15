
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int parse_object_sign(json_object *result, struct H3M_OD_ENTRY *od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_MESSAGE_BEARER *body 
		= (struct H3M_OD_BODY_DYNAMIC_MESSAGE_BEARER *)od_entry->body;

	if (body->mesg_size > 0) {
		json_object_object_add(result,
			"message", json_object_new_string((char *)body->mesg));
	}

	return 0;
}

