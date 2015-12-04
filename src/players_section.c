#include <windows.h>

#include "../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

static char *COLORS[] = {
	"red",
	"blue",
	"tan",
	"green",
	"orange",
	"purple",
	"teal",
	"pink"
};

int get_map_players_json(json_object *result, h3mlib_ctx_t ctx)
{
	struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	struct H3MLIB_META *meta = &ctx->meta;
	union H3M_PLAYER *player = NULL;
	int i = 0;

	for (i = 0; i < H3M_MAX_PLAYERS; ++i) {
		player = h3m->players[i];
		int active = 0;
		if (player->sod.can_be_human || player->sod.can_be_computer) {
			active = 1;
		}
		json_object *item;
		json_object *player_heroes;
		item = json_object_new_object();

		json_object_object_add(item, 
			"color", json_object_new_string(COLORS[i]));

		json_object_object_add(item, 
			"active", json_object_new_boolean(active));

		if (active) {
			
			//ai
			json_object_object_add(item, 
				"can_be_human", json_object_new_boolean(player->sod.can_be_human));

			json_object_object_add(item, 
				"can_be_computer", json_object_new_boolean(player->sod.can_be_computer));

			json_object_object_add(item, 
				"behavior", json_object_new_int(player->sod.behavior));

			json_object_object_add(item, 
				"allowed_alignments", json_object_new_int(player->sod.allowed_alignments));

			json_object_object_add(item, 
				"town_types", json_object_new_int(player->sod.town_types));

			json_object_object_add(item, 
				"town_conflux", json_object_new_int(player->sod.town_conflux));

			json_object_object_add(item, 
				"has_main_town", json_object_new_boolean(player->sod.has_main_town));
		}


		json_object_array_add(result, item);
	}

	return 0;
}
