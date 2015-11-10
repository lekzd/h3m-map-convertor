#include <windows.h>

#include "../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

int get_map_tiles_json(json_object *result, h3mlib_ctx_t ctx)
{
	struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	size_t mapSize = h3m_get_map_size(ctx);
	int levelsCount = h3m->bi.any.has_two_levels;
	int i = 0;
	int x = 0;
	int y = 0;
	int z = 0;

	for (z = 0; z <= levelsCount; ++z) {
		for (y = 0; y < mapSize; ++y) {
			for (x = 0; x < mapSize; ++x) {

				json_object *tile;
				tile = json_object_new_array();
				json_object_array_add(tile, json_object_new_int(h3m->tiles[i].terrain_type));
				json_object_array_add(tile, json_object_new_int(h3m->tiles[i].terrain_sprite));
				json_object_array_add(tile, json_object_new_int(h3m->tiles[i].river_type));
				json_object_array_add(tile, json_object_new_int(h3m->tiles[i].river_sprite));
				json_object_array_add(tile, json_object_new_int(h3m->tiles[i].road_type));
				json_object_array_add(tile, json_object_new_int(h3m->tiles[i].road_sprite));
				json_object_array_add(tile, json_object_new_int(h3m->tiles[i].mirroring));

				json_object_array_add(tile, json_object_new_int(x));
				json_object_array_add(tile, json_object_new_int(y));
				json_object_array_add(tile, json_object_new_int(z));

				json_object_array_add(result, tile);

				i++;
			}
			x = 0;
		}
		y = 0;
	}

	return 0;
}
