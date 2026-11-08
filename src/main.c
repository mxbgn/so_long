#include "raylib.h"
#include "so_long.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    Game map;
	if (getenv("TEST_MAP_ONLY")) {
        return validate_map(&map, argv[1]);
    }
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <map_file.ber>\n       %s --test\n", argv[0], argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "--test") == 0) {
        int test_ret = test_game();
        if (test_ret != 0)
            fprintf(stderr, "Test Error: Test suite failed with code %d\n", test_ret);
        return test_ret;
    }

    MapError ret = validate_map(&map, argv[1]);
    if (ret) {
        fprintf(stderr, "Error: %s\n", map_error_message(ret));
        return ret;
    }

    InitWindow(map.size.x * TILE_SIZE, map.size.y * TILE_SIZE, "so_long");
    SetTargetFPS(60);
    while (!WindowShouldClose() && !map.won) {
        play_game(&map);
        BeginDrawing();
        ClearBackground(BLACK);
        draw_map(&map);
        EndDrawing();
    }
    CloseWindow();
    free_map(&map);
    return 0;
}
