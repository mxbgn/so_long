#pragma once

#include <stddef.h>

#define TILE_SIZE 32

typedef struct {
    size_t x;
    size_t y;
} Coords;

typedef struct {
    char **map;
    Coords size;
    Coords player;
    size_t items;
    size_t moves;
} Game;

typedef enum {
    MAP_OK = 0,
    ERR_EXTENSION = 1,
    ERR_FILE = 2,
	ERR_READ = 3,
    ERR_SHAPE = 4,
    ERR_WALLS = 5,
    ERR_TILES = 6,
    ERR_PLAYER_EXIT = 7,
    ERR_PATH = 8,
    ERR_MEMORY = 9
} MapError;

int process_map(Game *level, char *path);
void free_map(Game *level);
