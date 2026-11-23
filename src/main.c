#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "so_long.h"
#include "render.h"

static const char *get_error_message(MapError error) {
    switch (error) {
    case MAP_OK:
        return "No error";
    case ERR_EXTENSION:
        return "Map file must have a .ber extension";
    case ERR_FILE:
        return "Could not open map file";
    case ERR_READ:
        return "Reading map file failed";
    case ERR_MEMORY:
        return "Memory allocation failed";
    case ERR_SHAPE:
        return "Map must be rectangular";
    case ERR_WALLS:
        return "Map must be surrounded by walls";
    case ERR_TILES:
        return "Map contains an invalid tile";
    case ERR_PLAYER_EXIT:
        return "Map must contain exactly one player and one exit";
    case ERR_PATH:
        return "Player cannot reach every collectible and the exit";
    default:
        return "Unknown map error";
    }
}

static int run_test(void) {
    int result = test_game();
    if (result != 0)
        fprintf(stderr, "Test suite failed with code %d\n", result);
    return result;
}

static int run_game(Game *game, char *path) {
    MapError error = validate_map(game, path);
    if (error != MAP_OK) {
        fprintf(stderr, "Error: %s\n", get_error_message(error));
        return error;
    }

    InitWindow(
        (int)(game->size.x * TILE_SIZE),
        (int)(game->size.y * TILE_SIZE),
        "so_long"
    );
    SetTargetFPS(60);
    game->assets = load_assets();

    while (!WindowShouldClose() && !game->won) {
        play_game(game);
        BeginDrawing();
        ClearBackground(BLACK);
        draw_game(game);
        EndDrawing();
    }

    unload_assets(game->assets);
    CloseWindow();
    free_map(game);
    return MAP_OK;
}

int main(int argc, char **argv) {
    Game game = {0};

    if (argc != 2) {
        fprintf(
            stderr,
            "Usage: %s <map_file.ber>\n       %s --test\n",
            argv[0],
            argv[0]
        );
        return 1;
    }
    if (strcmp(argv[1], "--test") == 0)
        return run_test();
    if (getenv("TEST_MAP_ONLY"))
        return validate_map(&game, argv[1]);

    return run_game(&game, argv[1]);
}
