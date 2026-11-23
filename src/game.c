#include "so_long.h"
#include "raylib.h"

void move_player(Game *game, int dx, int dy) {
    size_t new_x = game->player.x + dx;
    size_t new_y = game->player.y + dy;
    char target = game->map[new_y][new_x];

    if (target == '1')
        return;

	if (game->exit.x == game->player.x && game->exit.y == game->player.y)
		game->map[game->player.y][game->player.x] = 'E';
	else
		game->map[game->player.y][game->player.x] = '0';

    if (target == 'C')
        game->collectibles--;

    game->player.x = new_x;
    game->player.y = new_y;
    game->map[new_y][new_x] = 'P';

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
