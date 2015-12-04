#include "../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

#include "./objects/object_hero.h"

#include <stdio.h>

static int _get_bytes_array(json_object *result, char *bytes, int count)
{
    int i = 0;
    for (i = 0; i < count; ++i) {
        uint8_t byte = bytes[i];
        json_object_array_add(result,
            json_object_new_int(byte));
    }
    return 0;
}

static int _get_entity_coords_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
    const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
    struct H3M_OD_ENTRY *entry = NULL;
	entry = &h3m->od.entries[id];
			
    json_object_array_add(result,
        json_object_new_int(entry->header.x));

    json_object_array_add(result,
        json_object_new_int(entry->header.y));

    json_object_array_add(result,
        json_object_new_int(entry->header.z));

    return 0;
}

static void _set_type(json_object *result, char *type_name) 
{
    json_object* returnObj;
    if (!json_object_object_get_ex(result, "type", &returnObj)) {
        json_object_object_add(result, "type", json_object_new_string(type_name));
    }
}

static int _get_entity_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
    const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
    struct H3MLIB_META *meta = &((struct H3MLIB_CTX *)ctx)->meta;
    struct META_OD_ENTRY *meta_od_entry = NULL;
    struct H3M_OD_ENTRY *od_entry = NULL;

	meta_od_entry = &meta->od_entries[id];
	od_entry = &h3m->od.entries[id];

    switch (meta_od_entry->oa_type) {
        case H3M_OBJECT_PLACEHOLDER_HERO:
            _set_type(result, "hero_placeholder");
            break;

        case H3M_OBJECT_QUEST_GUARD:
            _set_type(result, "quest_guard");
            break;

        case H3M_OBJECT_PANDORAS_BOX:
            _set_type(result, "pandoras_box");
            break;

        case H3M_OBJECT_SIGN:
            _set_type(result, "sign");
        case H3M_OBJECT_OCEAN_BOTTLE:
            _set_type(result, "ocean_bottle");
            parse_object_sign(result, od_entry);
            break;

        case H3M_OBJECT_GARRISON:
        case H3M_OBJECT_GARRISON_ABSOD:
            _set_type(result, "garrison");
            break;

        case H3M_OBJECT_EVENT:
            _set_type(result, "event");
            break;

        case H3M_OBJECT_GRAIL:
            _set_type(result, "grail");             
            break;  

        case H3M_OBJECT_DWELLING:
        case H3M_OBJECT_DWELLING_ABSOD:
            _set_type(result, "dwelling");
        case H3M_OBJECT_LIGHTHOUSE:
            _set_type(result, "lighthouse");
        case H3M_OBJECT_RESOURCE_GENERATOR:
            _set_type(result, "mill");
        case H3M_OBJECT_SHIPYARD:
            _set_type(result, "shipyard");
        case H3M_OBJECT_ABANDONED_MINE_ABSOD:
            _set_type(result, "abandoned_mine");
            parse_object_static_flagged(result, od_entry);
            break;

        case H3M_OBJECT_GENERIC_BOAT:
            _set_type(result, "boat");
        case H3M_OBJECT_GENERIC_PASSABLE_TERRAIN:
        case H3M_OBJECT_GENERIC_PASSABLE_TERRAIN_SOD:
            _set_type(result, "passable_terrain");
        case H3M_OBJECT_GENERIC_IMPASSABLE_TERRAIN:
        case H3M_OBJECT_GENERIC_IMPASSABLE_TERRAIN_ABSOD:
            _set_type(result, "impassable_terrain");
        case H3M_OBJECT_GENERIC_VISITABLE:
        case H3M_OBJECT_GENERIC_VISITABLE_ABSOD:
            _set_type(result, "generic_visitable");
        case H3M_OBJECT_GENERIC_TREASURE:
            _set_type(result, "generic_treasure");
        case H3M_OBJECT_MONOLITH_TWO_WAY:
            _set_type(result, "monolith_two_way");
        case H3M_OBJECT_SUBTERRANEAN_GATE:
            _set_type(result, "subterranean_gate");
            break;

        case H3M_OBJECT_HERO:
        case H3M_OBJECT_RANDOM_HERO:
            _set_type(result, "hero");
        case H3M_OBJECT_PRISON:
            _set_type(result, "prison");
            parse_object_hero(result, od_entry);
            break;

        case H3M_OBJECT_TOWN:
        case H3M_OBJECT_TOWN_ABSOD:
            _set_type(result, "town");
            parse_object_town(result, od_entry);
            break;

        case H3M_OBJECT_RANDOM_DWELLING_ABSOD:
        case H3M_OBJECT_RANDOM_DWELLING_PRESET_ALIGNMENT_ABSOD:
        case H3M_OBJECT_RANDOM_DWELLING_PRESET_LEVEL_ABSOD:
            _set_type(result, "random_dwelling");
            parse_object_random_dwelling(result, od_entry, meta_od_entry);
            break;

        case H3M_OBJECT_MONSTER:
        case H3M_OBJECT_MONSTER_ABSOD:
            _set_type(result, "monster");
            break;

        case H3M_OBJECT_ARTIFACT:
        case H3M_OBJECT_ARTIFACT_AB:
        case H3M_OBJECT_ARTIFACT_SOD:
            _set_type(result, "artifact");
            break;

        case H3M_OBJECT_SHRINE:
            _set_type(result, "shrine");
            break;

        case H3M_OBJECT_SPELL_SCROLL:
            _set_type(result, "spell_scroll");
            break;

        case H3M_OBJECT_RESOURCE:
            _set_type(result, "resource");
            parse_object_resource(result, od_entry);
            break;

        case H3M_OBJECT_WITCH_HUT:
            _set_type(result, "witch_hut");
            break;

        case H3M_OBJECT_SEERS_HUT:
            _set_type(result, "seers_hut");
            break;

        case H3M_OBJECT_SCHOLAR:
            _set_type(result, "scholar");
            break;
    }
    return 0;
}

static int _get_object_json_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
    const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
    struct H3M_OA_ENTRY *oa_entry = NULL;
	struct H3M_OD_ENTRY *od_entry = NULL;
	od_entry = &h3m->od.entries[id];
	oa_entry = &h3m->oa.entries[od_entry->header.oa_index];

    json_object_object_add(result, 
        "id", json_object_new_int(id));

    json_object_object_add(result, 
        "texture", json_object_new_string((char *)oa_entry->header.def));

    json_object *entity_coords_data;
    entity_coords_data = json_object_new_array();
    _get_entity_coords_data(entity_coords_data, ctx, id);
    json_object_object_add(result, 
        "coords", entity_coords_data);

    json_object *passability_json;
    passability_json = json_object_new_array();
    _get_bytes_array(passability_json, (char *)oa_entry->body.passable,
		sizeof(oa_entry->body.passable));
    json_object_object_add(result, 
        "passability", passability_json);

    json_object *actions_json;
    actions_json = json_object_new_array();
    _get_bytes_array(actions_json, (char *)oa_entry->body.active,
		sizeof(oa_entry->body.active));
    json_object_object_add(result, 
        "actions", actions_json);

    json_object *entity_data;
    entity_data = json_object_new_object();
    _get_entity_data(entity_data, ctx, id);
    json_object_object_add(result, 
        "data", entity_data);

    json_object_object_add(result, 
        "object_class", json_object_new_int((int32_t)oa_entry->body.object_class));

    return 0;
}

int get_map_objects_json(json_object *result, h3mlib_ctx_t ctx)
{
    json_object *object;
    const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;

    for (unsigned int i = 0; i < h3m->od.count; ++i) {
        object = json_object_new_object();
        _get_object_json_data(object, ctx, i);
        json_object_array_add(result, object);
    }
    return 0;
}
