#include "raylib.h"
#include "so_long.h"
#include <stdio.h>


static const char *map_error_message(MapError err)
{
    switch (err) {
        case MAP_OK:          return "No error";
        case ERR_EXTENSION:   return "Map file must have a .ber extension";
        case ERR_FILE:        return "Could not open map file";
        case ERR_READ:        return "Error while reading map file";
        case ERR_MEMORY:      return "Memory allocation failed";
        case ERR_SHAPE:       return "Map must be rectangular and non-empty";
        case ERR_WALLS:       return "Map must be surrounded by walls";
        case ERR_TILES:       return "Map contains an invalid tile";
        case ERR_PLAYER_EXIT: return "Map must contain exactly one player and one exit";
        case ERR_PATH:        return "Player cannot reach every collectible and the exit";
        default:              return "Unknown map error";
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <map_file.ber>\n", argv[0]);
        return 1;
    }
    MapError ret = process_map(argv[1]);
    if (ret) {
        fprintf(stderr, "Error: %s\n", map_error_message(ret));
        return 1;
    }

    InitWindow(map->size.x * TILE_SIZE, map->size.y * TILE_SIZE, "so_long");
    while (!WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();
    }
    CloseVindow();
    return 0;
}
