#pragma once

#define TILE_SIZE 32

typedef struct {
    size_t x;
    size_t y;
} Coords;

typedef struct {
    char **map;
    Coordinates size;
    Coordinates player;
    size_t items;
    size_t moves;
} Game;

typedef enum {
    MAP_OK = 0,
    ERR_EXTENSION = 1,
    ERR_FILE = 2,
    ERR_SHAPE = 3,
    ERR_WALLS = 4,
    ERR_TILES = 5,
    ERR_PLAYER_EXIT = 6,
    ERR_PATH = 7,
    ERR_MEMORY = 8
} MapError;

int process_map(Game *level, char *path);
