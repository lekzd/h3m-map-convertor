// Created by Lekzd 02.11.2015

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "../../homm3tools/h3m/h3mlib/h3mlib.h"
#include "../../homm3tools/h3m/h3mlib/h3mlib_ctx.h"

#include "../3rdparty/json/random_seed.h"
#include "../3rdparty/json/json.h"

#include "properties_section.h"
#include "objects_section.h"
#include "players_section.h"
#include "tiles_section.h"

int main(int argc, char *argv[])
{
	json_object *map;
	json_object *properties;
	json_object *players;
	json_object *tiles;
	json_object *objects;

	h3mlib_ctx_t h3m = NULL;

	WCHAR path_u[MAX_PATH];

	_snwprintf(path_u, sizeof(path_u) / sizeof(WCHAR) - 1, L"%S", argv[1]);
	if (0 != h3m_read_u(&h3m, path_u)) {
		return 1;
	}

	map = json_object_new_object();

	//Map properties
	properties = json_object_new_object();
	get_map_properties(properties, h3m);

	//Map players
	players = json_object_new_array();
	get_map_players_json(players, h3m);

	//Map tiles
	tiles = json_object_new_array();
	get_map_tiles_json(tiles, h3m);

	//Map objects
	objects = json_object_new_array();
	get_map_objects_json(objects, h3m);

	//finalizing	
	// json_object_object_add(map, "props", properties);
	// json_object_object_add(map, "players", players);
	// json_object_object_add(map, "tiles", tiles);
	json_object_object_add(map, "objects", objects);

	//write string to file
	FILE *fp;

	fp = fopen(argv[2], "w+");

	fputs(json_object_to_json_string(map), fp);

	fclose(fp);
	//h3m_exit(&h3m);

    return 0;
}
