// Created by Lekzd 02.11.2015

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

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	json_object *map;
	json_object *properties;
	json_object *players;
	json_object *tiles;
	json_object *objects;

	h3mlib_ctx_t h3m = NULL;

	if (argc != 3) {
		return 1;
	}

	if (0 != h3m_read_u(&h3m, argv[1])) {
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
	json_object_object_add(map, "props", properties);
	json_object_object_add(map, "players", players);
	json_object_object_add(map, "tiles", tiles);
	json_object_object_add(map, "objects", objects);

	//write string to file
	FILE *fp;

	fp = _wfopen(argv[2], L"w+");

	fputs(json_object_to_json_string(map), fp);

	fclose(fp);
	//h3m_exit(&h3m);

    return 0;
}
