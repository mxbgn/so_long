#include "raylib.h"
#include "so_long.h"
#include <stdio.h>
#include <string.h>

static const char *map_error_message(MapError err)
{
    switch (err) {
        case MAP_OK:          return "No error";
        case ERR_EXTENSION:   return "Map file must have a .ber extension";
        case ERR_FILE:        return "Could not open map file";
        case ERR_READ:        return "Reading map file failed";
        case ERR_MEMORY:      return "Memory allocation failed";
        case ERR_SHAPE:       return "Map must be rectangular";
        case ERR_WALLS:       return "Map must be surrounded by walls";
        case ERR_TILES:       return "Map contains an invalid tile";
        case ERR_PLAYER_EXIT: return "Map must contain exactly one player and one exit";
        case ERR_PATH:        return "Player cannot reach every collectible and the exit";
        default:              return "Unknown map error";
    }
}

int main(int argc, char** argv) {
	int check_only = (argc == 3 && strcmp(argv[1], "--check") == 0);
    if (argc < 2 || argc > 3 || (argc == 3 && !check_only)) {
        fprintf(stderr, "Usage: %s [--check] <map_file.ber>\n", argv[0]);
        return 1;
    }

	Game map;
    char *map_path = check_only ? argv[2] : argv[1];
    MapError ret = process_map(&map, map_path);
    if (ret) {
        fprintf(stderr, "Error: %s\n", map_error_message(ret));
        return ret;
    }
	if (check_only) return 0;

	/*
    * InitWindow(map->size.x * TILE_SIZE, map->size.y * TILE_SIZE, "so_long");
    * while (!WindowShouldClose()) {
    *     BeginDrawing();
    *     EndDrawing();
    * }
    * CloseWindow();
	*/

	return 0;
}
