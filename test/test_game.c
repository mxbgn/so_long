#include "../src/so_long.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int test_game(void) {
printf("Running headless game logic tests with real map loading...\n");

    Game game;
    MapError err = validate_map(&game, "test/maps/valid/test_game_map.ber");
    assert(err == MAP_OK);
    assert(game.collectibles == 2);

    move_player(&game, 1, 0); // Move to (2,1) - empty space '0'
    assert(game.player.x == 2 && game.player.y == 1);

    move_player(&game, 1, 0); // Move to (3,1) - collectible 'C'
    assert(game.player.x == 3 && game.player.y == 1);
    assert(game.collectibles == 1);

    move_player(&game, 0, 1); // Move to (3,2) - empty space '0'
    assert(game.player.x == 3 && game.player.y == 2);

    move_player(&game, 0, 1); // FAIL move to (3,3) - exit 'E'
    assert(game.player.x == 3 && game.player.y == 2);

    move_player(&game, -1, 0); // Move to (2,2) - collectible 'C'
    assert(game.player.x == 2 && game.player.y == 2);
    assert(game.collectibles == 0);

    move_player(&game, 1, 0);
    move_player(&game, 0, 1); // Move to (3,3) - exit 'E'
	assert(game.won);

	free_map(&game);

    printf("All game logic tests passed successfully!\n");
    return 0;
}
