// Created by Lekzd 02.11.2015

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "3rdparty/json/random_seed.h"
#include "3rdparty/json/json.h"

char *COLORS[] = {
	"red",
	"blue",
	"tan",
	"green",
	"orange",
	"purple",
	"teal",
	"pink"
};

int _get_map_version(char *result, h3mlib_ctx_t ctx) 
{
	switch (h3m_get_format(ctx)) {
		case H3M_FORMAT_ROE: {
			strcpy(result, "RoE");
			break;
		}
		case H3M_FORMAT_AB: {
			strcpy(result, "AB");
			break;
		}
		case H3M_FORMAT_SOD: {
			strcpy(result, "SoD");
			break;
		}
		case H3M_FORMAT_WOG: {
			strcpy(result, "WoG");
			break;
		}
	}
	return 0;
}

int _get_has_caves(int *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	result = h3m->bi.any.has_two_levels;
	return 0;
}

int _get_difficulty_level(int *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	result = h3m->bi.any.difficulty;
	return 0;
}

int _get_map_name(WCHAR *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	strcpy(result, h3m->bi.any.name);
	return 0;
}

int _get_map_desc(WCHAR *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	strcpy(result, h3m->bi.any.desc);
	return 0;
}

int _get_player_heroes(json_object *result, struct H3M_PLAYER_AI_ABSOD *player_ai)
{	
	json_object *items;
	json_object *hero_item;
	items = json_object_new_array();
	hero_item = json_object_new_object();
	int i = 0;

	for (i = 0; i < player_ai->heroes_count; ++i) {
		// int player_type = player_ai->heroes[i].type;
		// json_object_object_add(hero_item, "type", 
		// 	json_object_new_int(player_type));

		// json_object_object_add(hero_item, "name", 
		// 	json_object_new_string(player_ai->heroes[i].name));

		json_object_array_add(items, hero_item);
	}
	
	json_object_array_add(result, items);
}

int _get_bytes_array(json_object *result, char *bytes)
{
	int count = 6;
	int i = 0;
	char *strcat(char *dest, const char *src);
	for (i = 0; i < count; ++i) {
		BYTE* byte = bytes[i];
		char byteStr[10];
		sprintf(byteStr, "%02X", byte);

		// strcat(bytesValue, byteStr[0]);
		// strcat(bytesValue, byteStr[1]);

		// strcpy(bytesValue, byteStr[(strlen(byteStr)-1)]);
		// strcpy(bytesValue, byteStr[(strlen(byteStr)-2)]);

		json_object_array_add(result, 
			json_object_new_string(byteStr));
	}
	return 0;
}

int _get_entity_coords_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	struct H3M_OD_ENTRY *entry = NULL;
	int i = 0;

	for (i = 0; i < h3m->od.count; ++i) {
		entry = &h3m->od.entries[i];
		if (entry->header.oa_index == id) {

			json_object_array_add(result, 
				json_object_new_int(entry->header.x));

			json_object_array_add(result, 
				json_object_new_int(entry->header.y));

			json_object_array_add(result, 
				json_object_new_int(entry->header.z));

			break;
		}
	}
}

int _get_entity_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	struct H3M_OD_ENTRY *entry = NULL;
	int i = 0;

	for (i = 0; i < h3m->od.count; ++i) {
		entry = &h3m->od.entries[i];
		if (entry->header.oa_index == id) {

			if (entry->body) {
				int body_size = sizeof(entry->body);
			}

			break;
		}
	}
}

int _get_object_json_data(json_object *result, h3mlib_ctx_t ctx, int id)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	struct H3M_OA_ENTRY *entry = NULL;
	entry = &h3m->oa.entries[id];

	json_object_object_add(result, "id", 
		json_object_new_int(id));

	json_object_object_add(result, "texture", 
		json_object_new_string((char *)entry->header.def));

	json_object *entity_coords_data;
	entity_coords_data = json_object_new_array();
	_get_entity_coords_data(entity_coords_data, ctx, id);
	json_object_object_add(result, "coords", entity_coords_data);

	json_object *passability_json;
	passability_json = json_object_new_array();
	_get_bytes_array(passability_json, (char *)entry->body.passable);
	json_object_object_add(result, "passability", passability_json);

	json_object *actions_json;
	actions_json = json_object_new_array();
	_get_bytes_array(actions_json, (char *)entry->body.active);
	json_object_object_add(result, "actions", actions_json);

	json_object *entity_data;
	entity_data = json_object_new_object();
	_get_entity_data(entity_data, ctx, id);
	json_object_object_add(result, "data", entity_data);

	json_object_object_add(result, "object_class", 
		json_object_new_int((int *)entry->body.object_class));

	return 0;
}

int _get_map_objects_json(json_object *result, h3mlib_ctx_t ctx)
{	
	json_object *object;
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	int i = 0;

	for (i = 0; i < h3m->oa.count; ++i) {
		object = json_object_new_object();
		_get_object_json_data(object, ctx, i);
		json_object_array_add(result, object);
	}
	return 0;
}

int _get_map_players_json(json_object *result, h3mlib_ctx_t ctx)
{
	const struct H3M *h3m = &((struct H3MLIB_CTX *)ctx)->h3m;
	const struct H3MLIB_META *meta = &ctx->meta;
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
		json_object_object_add(item, "color", json_object_new_string(COLORS[i]));
		json_object_object_add(item, "active", json_object_new_boolean(active));

		if (active) {
			
			//ai
			json_object_object_add(item, "can_be_human", 
				json_object_new_boolean(player->sod.can_be_human));

			json_object_object_add(item, "can_be_computer", 
				json_object_new_boolean(player->sod.can_be_computer));

			json_object_object_add(item, "behavior", 
				json_object_new_int(player->sod.behavior));

			json_object_object_add(item, "allowed_alignments", 
				json_object_new_int(player->sod.allowed_alignments));

			json_object_object_add(item, "town_types", 
				json_object_new_int(player->sod.town_types));

			json_object_object_add(item, "town_conflux", 
				json_object_new_int(player->sod.town_conflux));

			json_object_object_add(item, "has_main_town", 
				json_object_new_boolean(player->sod.has_main_town));

			player_heroes = json_object_new_array();
			_get_player_heroes(player_heroes, &h3m->player_ai[i]);
			json_object_object_add(item, "heroes", 
				json_object_new_array(player_heroes));
		}


		json_object_array_add(result, item);
	}

}

int _get_map_tiles_json(json_object *result, h3mlib_ctx_t ctx)
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

int main(void)
{

	json_object *map;
	json_object *properties;
	json_object *players;
	json_object *tiles;
	json_object *objects;

	h3mlib_ctx_t h3m = NULL;

	WCHAR MAP_FILE_NAME[] = L"D:\\HTTPD\\home\\homm3tools\\OUTPUT\\Debug\\big_test.h3m";

	if (0 != h3m_read_u(&h3m, MAP_FILE_NAME)) {
		return 1;
	}

	//struct H3M *p = &((struct H3MLIB_CTX *)h3m)->h3m;

	map = json_object_new_object();

	//Map properties
	json_object *propertyItem;
	properties = json_object_new_object();
	char mapVersion[4];
	_get_map_version(mapVersion, h3m);
	json_object_object_add(properties, "version", json_object_new_string(mapVersion));
	size_t mapSize = h3m_get_map_size(h3m);
	json_object_object_add(properties, "map_size", json_object_new_int(mapSize));
	int hasCaves = 0;
	_get_has_caves(hasCaves, h3m);
	json_object_object_add(properties, "has_caves", json_object_new_int(hasCaves));
	int difficulty = 0;
	_get_difficulty_level(difficulty, h3m);
	json_object_object_add(properties, "difficulty", json_object_new_int(difficulty));
	WCHAR name[255] = L"";
	_get_map_name(name, h3m);
	json_object_object_add(properties, "name", json_object_new_string(name));
	WCHAR desc[255] = L"";
	_get_map_desc(desc, h3m);
	json_object_object_add(properties, "descr", json_object_new_string(desc));

	//Map players
	players = json_object_new_array();
	_get_map_players_json(players, h3m);

	//Map tiles
	tiles = json_object_new_array();
	_get_map_tiles_json(tiles, h3m);

	//Map objects
	objects = json_object_new_array();
	_get_map_objects_json(objects, h3m);

	//finalizing
	// json_object_object_add(map, "props", properties);
	// json_object_object_add(map, "players", players);
	// json_object_object_add(map, "tiles", tiles);
	json_object_object_add(map, "objects", objects);

	//write string to file
	FILE *fp;

	fp = fopen("D:\\HTTPD\\home\\homm3tools\\OUTPUT\\Debug\\out.json", "w+");

	fputs(json_object_to_json_string(map), fp);

	fclose(fp);
	//h3m_exit(&h3m);

    return 0;
}

