
#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int parse_object_resource(json_object *result, struct H3M_OD_ENTRY *od_entry)
{
	struct H3M_OD_BODY_DYNAMIC_RESOURCE *body = NULL;
	body = od_entry->body;

    json_object_object_add(result,
            "quantity", json_object_new_int(body->quantity));
    
    json_object_object_add(result,
            "has_guardians", json_object_new_int(body->has_guardians));

	//TODO add guardians
    // body->guardians

	return 0;
}

