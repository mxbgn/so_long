#ifndef RENDER_H
# define RENDER_H

# include "so_long.h"
# include "raylib.h"

# define TILE_SIZE 32
# define SPRITE_SIZE 16

typedef struct Assets {
	Texture2D floors;
	Texture2D walls;
	Texture2D blocks;
	Texture2D player;
	Texture2D items;
} Assets;

typedef struct {
	const Texture2D *texture;
	Coords sprite;
	size_t frame_count;
} SpriteRef;

typedef enum {
	NEIGHBOR_N = 1 << 0,
	NEIGHBOR_E = 1 << 1,
	NEIGHBOR_S = 1 << 2,
	NEIGHBOR_W = 1 << 3,
	NEIGHBOR_NE = 1 << 4,
	NEIGHBOR_SE = 1 << 5,
	NEIGHBOR_SW = 1 << 6,
	NEIGHBOR_NW = 1 << 7
} NeighborMask;

# define PLAYER_SPRITE ((Coords){0, 0})
# define COLLECTIBLE_SPRITE ((Coords){0, 0})
# define EXIT_SPRITE ((Coords){3, 0})

# define FLOOR_DEFAULT ((Coords){10, 1})
# define FLOOR_SOLO ((Coords){0, 3})
# define FLOOR_N ((Coords){0, 2})
# define FLOOR_E ((Coords){1, 3})
# define FLOOR_S ((Coords){0, 0})
# define FLOOR_W ((Coords){3, 3})
# define FLOOR_N_E ((Coords){1, 2})
# define FLOOR_E_S ((Coords){1, 0})
# define FLOOR_S_W ((Coords){3, 0})
# define FLOOR_N_W ((Coords){3, 2})
# define FLOOR_N_S ((Coords){0, 1})
# define FLOOR_E_W ((Coords){2, 3})
# define FLOOR_N_E_NE ((Coords){8, 3})
# define FLOOR_E_S_SE ((Coords){8, 0})
# define FLOOR_S_W_SW ((Coords){11, 0})
# define FLOOR_N_W_NW ((Coords){11, 3})
# define FLOOR_N_E_S ((Coords){1, 1})
# define FLOOR_E_S_W ((Coords){2, 0})
# define FLOOR_N_S_W ((Coords){3, 1})
# define FLOOR_N_E_W ((Coords){2, 2})
# define FLOOR_N_E_S_NE ((Coords){4, 2})
# define FLOOR_N_E_S_SE ((Coords){4, 1})
# define FLOOR_E_S_W_SE ((Coords){5, 0})
# define FLOOR_E_S_W_SW ((Coords){6, 0})
# define FLOOR_N_S_W_SW ((Coords){7, 1})
# define FLOOR_N_S_W_NW ((Coords){7, 2})
# define FLOOR_N_E_W_NE ((Coords){5, 3})
# define FLOOR_N_E_W_NW ((Coords){6, 3})
# define FLOOR_N_E_S_NE_SE ((Coords){8, 1})
# define FLOOR_E_S_W_SE_SW ((Coords){10, 0})
# define FLOOR_N_S_W_SW_NW ((Coords){11, 2})
# define FLOOR_N_E_W_NE_NW ((Coords){9, 3})
# define FLOOR_N_E_S_W ((Coords){2, 1})
# define FLOOR_N_E_S_W_NE ((Coords){7, 0})
# define FLOOR_N_E_S_W_SE ((Coords){7, 3})
# define FLOOR_N_E_S_W_SW ((Coords){4, 3})
# define FLOOR_N_E_S_W_NW ((Coords){4, 0})
# define FLOOR_N_E_S_W_NE_SE ((Coords){8, 2})
# define FLOOR_N_E_S_W_SE_SW ((Coords){9, 0})
# define FLOOR_N_E_S_W_SW_NW ((Coords){11, 1})
# define FLOOR_N_E_S_W_NE_NW ((Coords){10, 3})
# define FLOOR_N_E_S_W_NE_SW ((Coords){9, 1})
# define FLOOR_N_E_S_W_SE_NW ((Coords){10, 2})
# define FLOOR_N_E_S_W_NE_SE_SW ((Coords){5, 1})
# define FLOOR_N_E_S_W_SE_SW_NW ((Coords){6, 1})
# define FLOOR_N_E_S_W_NE_SW_NW ((Coords){6, 2})
# define FLOOR_N_E_S_W_NE_SE_NW ((Coords){5, 2})
# define FLOOR_FULL ((Coords){9, 2})

# define WALL_SOLO ((Coords){0, 2})
# define WALL_N ((Coords){1, 2})
# define WALL_E ((Coords){1, 0})
# define WALL_S ((Coords){3, 0})
# define WALL_W ((Coords){3, 2})
# define WALL_N_E ((Coords){2, 2})
# define WALL_S_E ((Coords){1, 1})
# define WALL_S_W ((Coords){2, 0})
# define WALL_N_W ((Coords){3, 1})
# define WALL_NE ((Coords){0, 2})
# define WALL_SE ((Coords){1, 3})
# define WALL_SW ((Coords){0, 0})
# define WALL_NW ((Coords){3, 3})
# define WALL_NE_SW ((Coords){2, 3})
# define WALL_SE_NW ((Coords){0, 1})
# define WALL_FULL ((Coords){0, 3})

Assets *load_assets(void);
void unload_assets(Assets *assets);
void draw_game(const Game *game);

void init_wall_lut(const Assets *assets);
void init_floor_lut(const Assets *assets);
SpriteRef lookup_wall_sprite(const Game *game, size_t x, size_t y);
SpriteRef lookup_floor_sprite(const Game *game, size_t x, size_t y);

#endif
