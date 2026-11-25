#include "so_long.h"
#include "raylib.h"

static bool step_entity(const Game *game, Entity *ent, int dx, int dy) {
	if (dx < 0)
		ent->facing_left = true;
	else if (dx > 0)
		ent->facing_left = false;

	size_t new_x = ent->pos.x + dx;
	size_t new_y = ent->pos.y + dy;
	if (game->map[new_y][new_x] == '1')
		return false;

	ent->pos = (Coords){new_x, new_y};
	ent->moved = true;
	return true;
}

void move_player(Game *game, int dx, int dy) {
	Coords old = game->player.pos;
	if (!step_entity(game, &game->player, dx, dy))
		return;

	if (game->exit.x == old.x && game->exit.y == old.y)
		game->map[old.y][old.x] = 'E';
	else
		game->map[old.y][old.x] = '0';

	char target = game->map[game->player.pos.y][game->player.pos.x];
	if (target == 'C')
		game->collectibles--;
	game->map[game->player.pos.y][game->player.pos.x] = 'P';
	if (target == 'E' && game->collectibles == 0)
		game->won = true;
}

void play_game(Game *game) {
	if (IsKeyPressed(KEY_W))
		move_player(game, 0, -1);
	if (IsKeyPressed(KEY_S))
		move_player(game, 0, 1);
	if (IsKeyPressed(KEY_A))
		move_player(game, -1, 0);
	if (IsKeyPressed(KEY_D))
		move_player(game, 1, 0);
}
