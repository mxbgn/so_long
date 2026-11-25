#include "render.h"
#include <stdlib.h>

static void load_texture(Texture2D *texture, const char *path) {
	*texture = LoadTexture(path);
	SetTextureFilter(*texture, TEXTURE_FILTER_POINT);
}

Assets *load_assets(void) {
	Assets *assets = malloc(sizeof(*assets));
	load_texture(&assets->floors, "assets/SGQ_Dungeon/grounds_and_walls/grounds.png");
	load_texture(&assets->surground, "assets/SGQ_Dungeon/grounds_and_walls/surground.png");
	load_texture(&assets->walls, "assets/SGQ_Dungeon/grounds_and_walls/walls.png");
	load_texture(&assets->player, "assets/SGQ_Dungeon/characters/main/elf.png");
	load_texture(&assets->items, "assets/SGQ_Dungeon/props/pickup_items_animated.png");
	init_wall_lut(assets);
	init_floor_lut(assets);
	return assets;
}

void unload_assets(Assets *assets) {
	UnloadTexture(assets->floors);
	UnloadTexture(assets->surground);
	UnloadTexture(assets->walls);
	UnloadTexture(assets->player);
	UnloadTexture(assets->items);
	free(assets);
}

static size_t get_anim_frame(const SpriteRef *sprite, double time) {
	if (sprite->frame_count <= 1)
		return 0;
	return (size_t)(time * 5.0) % sprite->frame_count;
}

static float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

static void draw_entity(Entity *ent, Texture2D *texture, double time) {
	if (ent->moved) {
		ent->start_pos = ent->visual_pos;
		ent->move_start = time;
		ent->moved = false;
	}

	const double step_duration = 0.25;
	double elapsed = time - ent->move_start;
	SpriteRef sprite;
	size_t frame;
	if (ent->move_start > 0.0 && elapsed < step_duration) {
		float t = (float)(elapsed / step_duration);
		ent->visual_pos.x = lerp(ent->start_pos.x, (float)ent->pos.x, t);
		ent->visual_pos.y = lerp(ent->start_pos.y, (float)ent->pos.y, t);

		Coords row = ent->facing_left ? PLAYER_RUN_LEFT : PLAYER_RUN_RIGHT;
		sprite = (SpriteRef){texture, row, 4};
		frame = (size_t)(t * sprite.frame_count);
	} else {
		ent->visual_pos = (Vector2){(float)ent->pos.x, (float)ent->pos.y};
		Coords row = ent->facing_left ? PLAYER_IDLE_LEFT : PLAYER_IDLE_RIGHT;
		sprite = (SpriteRef){texture, row, 3};
		frame = get_anim_frame(&sprite, elapsed - step_duration);
	}

	draw_sprite(&sprite, ent->visual_pos.x, ent->visual_pos.y, frame);
}

void draw_sprite(const SpriteRef *sprite, float x, float y, size_t frame) {
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

	DrawTexturePro(*sprite->texture, source, destination,
		(Vector2){0.0f, 0.0f}, 0.0f, WHITE);
}

void draw_game(Game *game, LevelType type) {
	double time = GetTime();
	SpriteRef sprite;

	for (size_t y = 0; y < game->size.y; y++) {
		for (size_t x = 0; x < game->size.x; x++) {
			char tile = game->map[y][x];
			if (tile == '1')
				sprite = (SpriteRef){&game->assets->floors, FLOOR_DEFAULT, 1};
			else
				sprite = lookup_floor_sprite(game, x, y, type);
			draw_sprite(&sprite, (float)x, (float)y, 0);

			switch (tile) {
			case '1':
				sprite = lookup_wall_sprite(game, x, y, type);
				break;
			case 'P':
				if (x != game->exit.x || y != game->exit.y)
					continue;
				sprite = (SpriteRef){&game->assets->walls, EXIT_SPRITE, 1};
				break;
			case 'C':
				sprite = (SpriteRef){&game->assets->items, COLLECTIBLE_SPRITE, 4};
				break;
			case 'E':
				sprite = (SpriteRef){&game->assets->walls, EXIT_SPRITE, 1};
				break;
			default:
				continue;
			}

			draw_sprite(&sprite, (float)x, (float)y, get_anim_frame(&sprite, time));
		}
	}

	draw_entity(&game->player, &game->assets->player, time);
}
