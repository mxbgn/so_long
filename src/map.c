#include "so_long.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static int find_path_bfs(Game *level) {
    size_t total = level->size.x * level->size.y;
    char *visited = calloc(total, sizeof(char));
    if (!visited) return ERR_MEMORY;

    Coords *queue = malloc(total * sizeof(Coords));
    if (!queue) {
        free(visited);
        return ERR_MEMORY;
    }

    size_t rear = 0, front = 0;
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};

    queue[rear++] = (Coords){level->player.x, level->player.y};
    visited[level->player.y * level->size.x + level->player.x] = 1;

    while (front < rear) {
        Coords cur = queue[front++];
        for (int d = 0; d < 4; d++) {
            int x = cur.x + dx[d];
            int y = cur.y + dy[d];
            if (level->map[y][x] != '1' && !visited[y * level->size.x + x]) {
                visited[y * level->size.x + x] = 1;
                queue[rear++] = (Coords){x, y};
            }
        }
    }

    int result = MAP_OK;
    for (size_t i = 0; i < total; i++) {
        int x = i % level->size.x;
        int y = i / level->size.x;
        if ((level->map[y][x] == 'C' || level->map[y][x] == 'E') && !visited[i]) {
            result = ERR_PATH;
            break;
        }
    }

    free(visited);
    free(queue);
    return result;
}

static int read_map_lines(FILE *fp, Game *level) {
    ssize_t bytes;
    size_t map_height = 64;
    
    level->map = calloc(map_height, sizeof *level->map);
    if (!level->map) return ERR_MEMORY;

    errno = 0;
    for (size_t i = 0, buf_size = 0; (bytes = getline(&level->map[i], &buf_size, fp)) > 0; i++) {
        if (level->map[i][bytes - 1] == '\n') level->map[i][--bytes] = '\0';
        if (bytes > 0 && level->map[i][bytes - 1] == '\r') level->map[i][--bytes] = '\0'; // Remove newline on windows and MAC

        if (level->size.x == 0) level->size.x = bytes;
        if (level->size.x != bytes || !bytes) return ERR_SHAPE;

        buf_size = 0;
        level->size.y++;

        if (level->size.y == map_height) {
            char **new_map = realloc(level->map, map_height * 2 * sizeof *level->map);
            if (!new_map) return ERR_MEMORY;
            memset(new_map + map_height, 0, map_height * sizeof *new_map);
            map_height *= 2;
            level->map = new_map;
        }
    }

    if (errno == ENOMEM) return ERR_MEMORY;
    if (ferror(fp)) return ERR_READ;
    if (!level->size.x || !level->size.y) return ERR_SHAPE;

    return MAP_OK;
}

static int validate_walls(const Game *level) {
    for (size_t x = 0; x < level->size.x; x++) {
        if (level->map[0][x] != '1' || level->map[level->size.y - 1][x] != '1')
            return ERR_WALLS;
    }
    for (size_t y = 0; y < level->size.y; y++) {
        if (level->map[y][0] != '1' || level->map[y][level->size.x - 1] != '1')
            return ERR_WALLS;
    }
    return MAP_OK;
}

static int validate_contents(Game *level) {
    size_t player = 0, exit = 0;
    level->items = 0;

    for (size_t y = 1; y < level->size.y - 1; y++) {
        for (size_t x = 1; x < level->size.x - 1; x++) {
            char tile = level->map[y][x];
            if (tile == 'C') {
                level->items++;
            } else if (tile == 'P') {
                player++;
                level->player.x = x;
                level->player.y = y;
            } else if (tile == 'E') {
                exit++;
            } else if (tile != '0' && tile != '1') {
                return ERR_TILES;
            }
        }
    }

    if (player != 1 || exit != 1) return ERR_PLAYER_EXIT;
    return MAP_OK;
}

int process_map(Game *level, char *path) {
    size_t path_size = strlen(path);
    if (path_size < 4 || strcmp(path + path_size - 4, ".ber") != 0)
        return ERR_EXTENSION;

    FILE *fp = fopen(path, "r");
    if (!fp) return ERR_FILE;

    level->size.x = 0;
    level->size.y = 0;

    int status = read_map_lines(fp, level);
    fclose(fp);
    if (status != MAP_OK) goto err_cleanup;
    if ((status = validate_walls(level)) != MAP_OK) goto err_cleanup;
    if ((status = validate_contents(level)) != MAP_OK) goto err_cleanup;
    
    return find_path_bfs(level);

err_cleanup:
    free_map(level);
    return status;
}

void free_map(Game *level) {
	for (size_t i = 0; i < level->size.y; i++)
		free(level->map[i]);
	free(level->map);
}
