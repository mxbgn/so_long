#include "so_long.h"
#include "render.h"
#include <stdint.h>

static SpriteRef g_wall_lut[256];
static SpriteRef g_floor_lut[256];

static void set_lut_variants(uint8_t req, uint8_t opt, SpriteRef *lut, SpriteRef sprite) {
	for (int i = 0; i < 256; i++) {
		if ((i & ~opt) == req)
			lut[i] = sprite;
	}
}

void init_wall_lut(const Assets *assets) {
	// Default texture for walls with 3 or 4 empty cardinals
	for (int i = 0; i < 256; i++) {
		g_wall_lut[i] = (SpriteRef){
			.texture = &assets->walls,
			.sprite = WALL_SOLO,
			.frame_count = 1,
		};
	}

	// All cardinals are walls
	g_wall_lut[0] = (SpriteRef){&assets->walls, WALL_FULL, 1};

	g_wall_lut[NEIGHBOR_NE] = (SpriteRef){&assets->walls, WALL_NE, 1};
	g_wall_lut[NEIGHBOR_SE] = (SpriteRef){&assets->walls, WALL_SE, 1};
	g_wall_lut[NEIGHBOR_SW] = (SpriteRef){&assets->walls, WALL_SW, 1};
	g_wall_lut[NEIGHBOR_NW] = (SpriteRef){&assets->walls, WALL_NW, 1};

	g_wall_lut[NEIGHBOR_NE | NEIGHBOR_SE] = (SpriteRef){&assets->walls, WALL_E, 1};
	g_wall_lut[NEIGHBOR_SE | NEIGHBOR_SW] = (SpriteRef){&assets->walls, WALL_S, 1};
	g_wall_lut[NEIGHBOR_SW | NEIGHBOR_NW] = (SpriteRef){&assets->walls, WALL_W, 1};
	g_wall_lut[NEIGHBOR_NE | NEIGHBOR_NW] = (SpriteRef){&assets->walls, WALL_N, 1};
	g_wall_lut[NEIGHBOR_NE | NEIGHBOR_SW] = (SpriteRef){&assets->walls, WALL_NE_SW, 1};
	g_wall_lut[NEIGHBOR_SE | NEIGHBOR_NW] = (SpriteRef){&assets->walls, WALL_SE_NW, 1};

	g_wall_lut[NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW] = (SpriteRef){&assets->walls, WALL_S_E, 1};
	g_wall_lut[NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW] = (SpriteRef){&assets->walls, WALL_S_W, 1};
	g_wall_lut[NEIGHBOR_NE | NEIGHBOR_SW | NEIGHBOR_NW] = (SpriteRef){&assets->walls, WALL_N_W, 1};
	g_wall_lut[NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_NW] = (SpriteRef){&assets->walls, WALL_N_E, 1};

	// 1 empty cardinal
	set_lut_variants(NEIGHBOR_N, NEIGHBOR_NE | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_N, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_SE, NEIGHBOR_NE | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_N_E, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_SW, NEIGHBOR_NE | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_N_W, 1});

	set_lut_variants(NEIGHBOR_E, NEIGHBOR_NE | NEIGHBOR_SE, g_wall_lut, (SpriteRef){&assets->walls, WALL_E, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_SW, NEIGHBOR_NE | NEIGHBOR_SE, g_wall_lut, (SpriteRef){&assets->walls, WALL_S_E, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_NW, NEIGHBOR_NE | NEIGHBOR_SE, g_wall_lut, (SpriteRef){&assets->walls, WALL_N_E, 1});

	set_lut_variants(NEIGHBOR_S, NEIGHBOR_SE | NEIGHBOR_SW, g_wall_lut, (SpriteRef){&assets->walls, WALL_S, 1});
	set_lut_variants(NEIGHBOR_S | NEIGHBOR_NE, NEIGHBOR_SE | NEIGHBOR_SW, g_wall_lut, (SpriteRef){&assets->walls, WALL_S_E, 1});
	set_lut_variants(NEIGHBOR_S | NEIGHBOR_NW, NEIGHBOR_SE | NEIGHBOR_SW, g_wall_lut, (SpriteRef){&assets->walls, WALL_S_W, 1});

	set_lut_variants(NEIGHBOR_W, NEIGHBOR_SW | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_W, 1});
	set_lut_variants(NEIGHBOR_W | NEIGHBOR_NE, NEIGHBOR_SW | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_N_W, 1});
	set_lut_variants(NEIGHBOR_W | NEIGHBOR_SE, NEIGHBOR_SW | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_S_W, 1});

	// 2 empty cardinals
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_N_E, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_S, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW, g_wall_lut, (SpriteRef){&assets->walls, WALL_S_E, 1});
	set_lut_variants(NEIGHBOR_S | NEIGHBOR_W, NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_S_W, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_W, NEIGHBOR_NE | NEIGHBOR_SW | NEIGHBOR_NW, g_wall_lut, (SpriteRef){&assets->walls, WALL_N_W, 1});
}

void init_floor_lut(const Assets *assets) {
	for (int i = 0; i < 256; i++) {
		g_floor_lut[i] = (SpriteRef){
			.texture = &assets->floors,
			.sprite = FLOOR_DEFAULT,
			.frame_count = 1,
		};
	}

	// All cardinals are walls, diagonals are masked
	g_floor_lut[0] = (SpriteRef){&assets->floors, FLOOR_SOLO, 1};

	// 1 empty cardinal, diagonals are masked
	g_floor_lut[NEIGHBOR_N] = (SpriteRef){&assets->floors, FLOOR_N, 1};
	g_floor_lut[NEIGHBOR_E] = (SpriteRef){&assets->floors, FLOOR_E, 1};
	g_floor_lut[NEIGHBOR_S] = (SpriteRef){&assets->floors, FLOOR_S, 1};
	g_floor_lut[NEIGHBOR_W] = (SpriteRef){&assets->floors, FLOOR_W, 1};

	// 2 empty cardinals
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E, 1});
	set_lut_variants(NEIGHBOR_S | NEIGHBOR_E, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_E_S, 1});
	set_lut_variants(NEIGHBOR_S | NEIGHBOR_W, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_S_W, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_W, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_W, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_S, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_S, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_W, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_E_W, 1});

	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_NE, NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_NE, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_SE, NEIGHBOR_NE | NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_E_S_SE, 1});
	set_lut_variants(NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SW, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_S_W_SW, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_W | NEIGHBOR_NW, NEIGHBOR_NE | NEIGHBOR_SE | NEIGHBOR_SW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_W_NW, 1});

	// 3 empty cardinals
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S, NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_S, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W, NEIGHBOR_NE | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_E_S_W, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_S | NEIGHBOR_W, NEIGHBOR_NE | NEIGHBOR_SE, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_S_W, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_W, NEIGHBOR_SE | NEIGHBOR_SW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_W, 1});
	
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_NE, NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_S_NE, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_SE, NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_S_SE, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SE, NEIGHBOR_NE | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_E_S_W_SE, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SW, NEIGHBOR_NE | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_E_S_W_SW, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SW, NEIGHBOR_NE | NEIGHBOR_SE, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_S_W_SW, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NW, NEIGHBOR_NE | NEIGHBOR_SE, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_S_W_NW, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_W | NEIGHBOR_NE, NEIGHBOR_SE | NEIGHBOR_SW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_W_NE, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_W | NEIGHBOR_NW, NEIGHBOR_SE | NEIGHBOR_SW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_W_NW, 1});

	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_NE | NEIGHBOR_SE, NEIGHBOR_SW | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_S_NE_SE, 1});
	set_lut_variants(NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SE | NEIGHBOR_SW, NEIGHBOR_NE | NEIGHBOR_NW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_E_S_W_SE_SW, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SW | NEIGHBOR_NW, NEIGHBOR_NE | NEIGHBOR_SE, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_S_W_SW_NW, 1});
	set_lut_variants(NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_W | NEIGHBOR_NE | NEIGHBOR_NW, NEIGHBOR_SE | NEIGHBOR_SW, g_floor_lut, (SpriteRef){&assets->floors, FLOOR_N_E_W_NE_NW, 1});

	// 4 empty cardinal
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NE, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SE] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_SE, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_SW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NW, 1};

	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE
		| NEIGHBOR_SE] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NE_SE, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SE
		| NEIGHBOR_SW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_SE_SW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SW
		| NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_SW_NW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE
		| NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NE_NW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE
		| NEIGHBOR_SW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NE_SW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SE
		| NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_SE_NW, 1};

	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE
		| NEIGHBOR_SE | NEIGHBOR_SW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NE_SE_SW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_SE
		| NEIGHBOR_SW | NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_SE_SW_NW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE
		| NEIGHBOR_SW | NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NE_SW_NW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE
		| NEIGHBOR_SE | NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_N_E_S_W_NE_SE_NW, 1};
	g_floor_lut[NEIGHBOR_N | NEIGHBOR_E | NEIGHBOR_S | NEIGHBOR_W | NEIGHBOR_NE
		| NEIGHBOR_SE | NEIGHBOR_SW | NEIGHBOR_NW] =
		(SpriteRef){&assets->floors, FLOOR_FULL, 1};
}

static size_t	tile_pick(const Game *game, size_t x, size_t y, size_t variants, size_t chance_pct) {
	if (variants <= 1)
		return (0);
	unsigned int h = (unsigned int)((x + game->size.x) * 374761393u + (y - game->size.y)  * 668265263u);
	h = (h ^ (h >> 13)) * 1274126177u;
	h = h ^ (h >> 17);
	if (h % 100 >= chance_pct)
		return (0);
	return (1 + (h / 100) % (variants - 1));
}

static NeighborMask get_neighbors(const Game *game, size_t x, size_t y) {
	NeighborMask neighbors = 0;

	if (y > 0 && game->map[y - 1][x] != '1')
		neighbors |= NEIGHBOR_N;
	if (y + 1 < game->size.y && game->map[y + 1][x] != '1')
		neighbors |= NEIGHBOR_S;
	if (x + 1 < game->size.x && game->map[y][x + 1] != '1')
		neighbors |= NEIGHBOR_E;
	if (x > 0 && game->map[y][x - 1] != '1')
		neighbors |= NEIGHBOR_W;

	if (y > 0 && x + 1 < game->size.x && game->map[y - 1][x + 1] != '1')
		neighbors |= NEIGHBOR_NE;
	if (y + 1 < game->size.y && x + 1 < game->size.x && game->map[y + 1][x + 1] != '1')
		neighbors |= NEIGHBOR_SE;
	if (y + 1 < game->size.y && x > 0 && game->map[y + 1][x - 1] != '1')
		neighbors |= NEIGHBOR_SW;
	if (y > 0 && x > 0 && game->map[y - 1][x - 1] != '1')
		neighbors |= NEIGHBOR_NW;
	return (neighbors);
}

SpriteRef lookup_floor_sprite(const Game *game, size_t x, size_t y, LevelType type) {
	// mask diagonals on floor tiles with 0 or 1 cardinal empty neighbor
	NeighborMask neighbors = get_neighbors(game, x, y);
	unsigned int cardinals = (unsigned int)neighbors & 0x0Fu;
	if ((cardinals & (cardinals - 1u)) == 0u)
		neighbors = (NeighborMask)((unsigned int)neighbors & 0x0Fu);

	SpriteRef ref = g_floor_lut[neighbors];
	if (type == TYPE_1) {
		draw_sprite(&ref, (float)x, (float)y, 0);

		size_t pick = tile_pick(game, x, y, 3u, 40u);
		if (neighbors == 0xFFu) {
			if (pick == 1)
				ref.sprite = SURGROUND_VAR_1;
			else if (pick == 2)
				ref.sprite = SURGROUND_VAR_2;
		}
		ref.texture = &game->assets->surground;
	}
	else if (type == TYPE_2) {
		size_t pick = tile_pick(game, x, y, 2u, 40u);
		if (pick && ref.sprite.x == FLOOR_FULL.x && ref.sprite.y == FLOOR_FULL.y) {
			ref.sprite.x += 3;
			ref.sprite.y -= 2;
		}
		ref.sprite.y += 12;
	}
	return ref;
}

SpriteRef lookup_wall_sprite(const Game *game, size_t x, size_t y, LevelType type) {
	SpriteRef ref = g_wall_lut[get_neighbors(game, x, y)];
	size_t pick;
	if (type == TYPE_3)
		pick = tile_pick(game, x, y, 4u, 20u);
	else
		pick = tile_pick(game, x, y, 5u, 20u);

	if (ref.sprite.x == WALL_N.x && ref.sprite.y == WALL_N.y && pick > 0) {
		ref.sprite.x += pick + 3;
		ref.sprite.y++;
	} else if (ref.sprite.x == WALL_E.x && ref.sprite.y == WALL_E.y && pick > 0) {
		ref.sprite.x += pick + 3;
		ref.sprite.y += 2;
	} else if (ref.sprite.x == WALL_S.x && ref.sprite.y == WALL_S.y && pick > 0)
		ref.sprite.x += pick + 1;
	else if (ref.sprite.x == WALL_W.x && ref.sprite.y == WALL_W.y && pick > 0) {
		ref.sprite.x += pick + 1;
		ref.sprite.y--;
	}

	if (ref.sprite.x != WALL_SOLO.x || ref.sprite.y != WALL_SOLO.y) {
		if (type == TYPE_2)
			ref.sprite.y += 5;
		else if (type == TYPE_3)
			ref.sprite.y += 10;
	}
	return ref;
}
