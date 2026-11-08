#include "so_long.h"

static Color get_tile_color(char tile) {
    if (tile == '1')
        return DARKGRAY;
    if (tile == 'P')
        return BLUE;
    if (tile == 'C')
        return GOLD;
    if (tile == 'E')
        return GREEN;
    return LIGHTGRAY;
}

void draw_map(Game *game) {
    for (int y = 0; y < game->size.y; y++) {
        for (int x = 0; x < game->size.x; x++) {
            char tile = game->map[y][x];
            DrawRectangle(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                get_tile_color(tile)
            );
        }
    }
}
