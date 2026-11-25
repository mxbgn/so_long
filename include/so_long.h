#ifndef SO_LONG_H
# define SO_LONG_H

# include <stddef.h>
# include <stdbool.h>
#include "raylib.h"

typedef struct {
	size_t x;
	size_t y;
} Coords;

typedef struct {
	Coords  pos;
	Vector2 start_pos;
	Vector2 visual_pos;
	double  move_start;
	bool    moved;
	bool    facing_left;
} Entity;

typedef struct Assets Assets;
typedef struct {
	char **map;
	Coords size;
	Entity player;
	Coords exit;
	size_t collectibles;
	Assets *assets;
	bool won;
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

int validate_map(Game *level, char *path);
void free_map(Game *level);
void play_game(Game *level);
void move_player(Game *game, int dx, int dy);
int test_game(void);

#endif
