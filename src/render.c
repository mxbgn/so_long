#include "render.h"
#include <stdlib.h>

static void load_texture(Texture2D *texture, const char *path) {
	*texture = LoadTexture(path);
	SetTextureFilter(*texture, TEXTURE_FILTER_POINT);
}

Assets *load_assets(void) {
	Assets *assets = malloc(sizeof(*assets));
	load_texture(&assets->floors, "assets/SGQ_Dungeon/grounds_and_walls/grounds.png");
	load_texture(&assets->walls, "assets/SGQ_Dungeon/grounds_and_walls/walls.png");
	load_texture(&assets->blocks, "assets/SGQ_Dungeon/grounds_and_walls/blocks.png");
	load_texture(&assets->player, "assets/SGQ_Dungeon/characters/main/elf.png");
	load_texture(&assets->items, "assets/SGQ_Dungeon/props/pickup_items_animated.png");
	init_wall_lut(assets);
	init_floor_lut(assets);
	return assets;
}

void unload_assets(Assets *assets) {
	UnloadTexture(assets->floors);
	UnloadTexture(assets->walls);
	UnloadTexture(assets->blocks);
	UnloadTexture(assets->player);
	UnloadTexture(assets->items);
	free(assets);
}

static void draw_sprite(const SpriteRef *sprite, size_t x, size_t y, double time) {
	size_t frame = 0;
	if (sprite->frame_count > 1)
		frame = (size_t)(time * 8.0) % sprite->frame_count;

	Rectangle source = (Rectangle){
		.x = (sprite->sprite.x + frame) * SPRITE_SIZE,
		.y = sprite->sprite.y * SPRITE_SIZE,
		.width = SPRITE_SIZE,
		.height = SPRITE_SIZE,
	};

	Rectangle destination = (Rectangle){
		.x = x * TILE_SIZE,
		.y = y * TILE_SIZE,
		.width = TILE_SIZE,
		.height = TILE_SIZE,
	};

	DrawTexturePro(*sprite->texture, source, destination, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
}

void draw_game(const Game *game) {
	double time = GetTime();
	SpriteRef sprite;

	for (size_t y = 0; y < game->size.y; y++) {
		for (size_t x = 0; x < game->size.x; x++) {
			char tile = game->map[y][x];
			if (tile == '1')
				sprite = (SpriteRef){&game->assets->floors, FLOOR_DEFAULT, 1};
			else
				sprite = lookup_floor_sprite(game, x, y);
			draw_sprite(&sprite, x, y, time);
			switch (tile) {
			case '1':
				sprite = lookup_wall_sprite(game, x, y);
				break;
			case 'P':
				if (x == game->exit.x && y == game->exit.y) {
					sprite = (SpriteRef){&game->assets->blocks, EXIT_SPRITE, 1};
					draw_sprite(&sprite, x, y, time);
				}
				sprite = (SpriteRef){&game->assets->player, PLAYER_SPRITE, 1};
				break;
			case 'C':
				sprite = (SpriteRef){&game->assets->items, COLLECTIBLE_SPRITE, 4};
				break;
			case 'E':
				sprite = (SpriteRef){&game->assets->blocks, EXIT_SPRITE, 1};
				break;
			default:
				continue;
			}
			draw_sprite(&sprite, x, y, time);
		}
	}
}
