#ifndef __OBJECT_MONSTER_H_DEF__
#define __OBJECT_MONSTER_H_DEF__

#include "../../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../../3rdparty/json/random_seed.h"
#include "../../3rdparty/json/json.h"

int parse_object_monster(json_object *result, struct H3M_OD_ENTRY *od_entry, struct META_OD_ENTRY *meta_od_entry);

#endif
