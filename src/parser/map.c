#include "so_long.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_MAP_CAPACITY 64

static bool is_walkable(char tile) {
    return tile == '0' || tile == 'C' || tile == 'E' || tile == 'P';
}

static int find_path_bfs(Game *level) {
    size_t total = level->size.x * level->size.y;
    char *visited = calloc(total, sizeof(*visited));
    Coords *queue = malloc(total * sizeof(*queue));
    if (!visited || !queue) {
        free(visited);
        free(queue);
        return ERR_MEMORY;
    }

    static const int offsets[4][2] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    size_t front = 0;
    size_t rear = 0;
    queue[rear++] = level->player.pos;
    visited[level->player.pos.y * level->size.x + level->player.pos.x] = 1;
    while (front < rear) {
        Coords current = queue[front++];
        for (size_t direction = 0; direction < 4; direction++) {
            size_t x = current.x + offsets[direction][0];
            size_t y = current.y + offsets[direction][1];
			size_t index = y * level->size.x + x;

            if (x >= level->size.x || y >= level->size.y || !is_walkable(level->map[y][x]) || visited[index])
                continue;
            visited[index] = 1;
			queue[rear++] = (Coords){x, y};
        }
    }

    int result = MAP_OK;
    for (size_t y = 0; y < level->size.y && result == MAP_OK; y++) {
        for (size_t x = 0; x < level->size.x; x++) {
            size_t index = y * level->size.x + x;
            char tile = level->map[y][x];
            if ((tile == 'C' || tile == 'E') && !visited[index]) {
                result = ERR_PATH;
                break;
            }
        }
    }

    free(visited);
    free(queue);
    return result;
}

static int read_map_lines(FILE *file, Game *level) {
    size_t capacity = INITIAL_MAP_CAPACITY;
    ssize_t bytes;
    level->map = calloc(capacity, sizeof(*level->map));
    if (!level->map)
        return ERR_MEMORY;

    for (size_t y = 0;; y++) {
        size_t line_capacity = 0;
        char *line = NULL;
        bytes = getline(&line, &line_capacity, file);
        if (bytes < 0) {
            free(line);
            break;
        }

        if (bytes > 0 && line[bytes - 1] == '\n')
            line[--bytes] = '\0';
        if (bytes > 0 && line[bytes - 1] == '\r')
            line[--bytes] = '\0';
        if (level->size.x == 0)
            level->size.x = bytes;
        if (bytes == 0 || level->size.x != bytes) {
            free(line);
            return ERR_SHAPE;
        }

        level->map[y] = line;
        level->size.y++;
        if (level->size.y == capacity) {
            char **larger_map = realloc(level->map, capacity * 2 * sizeof(*larger_map));
            if (!larger_map)
                return ERR_MEMORY;
            memset(larger_map + capacity, 0, capacity * sizeof(*larger_map));
            level->map = larger_map;
            capacity *= 2;
        }
    }

    if (ferror(file))
        return ERR_READ;
    if (level->size.x == 0 || level->size.y == 0)
        return ERR_SHAPE;

    return MAP_OK;
}

static int validate_walls(const Game *level) {
    for (size_t x = 0; x < level->size.x; x++) {
        if (level->map[0][x] != '1')
            return ERR_WALLS;
        if (level->map[level->size.y - 1][x] != '1')
            return ERR_WALLS;
    }

    for (size_t y = 0; y < level->size.y; y++) {
        if (level->map[y][0] != '1')
            return ERR_WALLS;
        if (level->map[y][level->size.x - 1] != '1')
            return ERR_WALLS;
    }

    return MAP_OK;
}

static int validate_contents(Game *level) {
    size_t players = 0;
    size_t exits = 0;
    level->collectibles = 0;

    for (size_t y = 1; y < level->size.y - 1; y++) {
        for (size_t x = 1; x < level->size.x - 1; x++) {
            char tile = level->map[y][x];
            if (tile == 'C')
                level->collectibles++;
            else if (tile == 'P') {
                players++;
                level->player.pos.x = x;
                level->player.pos.y = y;
            } else if (tile == 'E') {
                exits++;
				level->exit.x = x;
				level->exit.y = y;
            } else if (tile != '0' && tile != '1')
                return ERR_TILES;
        }
    }

    if (players != 1 || exits != 1)
        return ERR_PLAYER_EXIT;

    return MAP_OK;
}

int validate_map(Game *level, char *path) {
    size_t path_length = strlen(path);
    if (path_length < 4 || strcmp(path + path_length - 4, ".ber") != 0)
        return ERR_EXTENSION;
    FILE *file = fopen(path, "r");
    if (!file)
        return ERR_FILE;

    int status = read_map_lines(file, level);
    fclose(file);
    if (status != MAP_OK)
        goto cleanup;
    status = validate_walls(level);
    if (status != MAP_OK)
        goto cleanup;
    status = validate_contents(level);
    if (status != MAP_OK)
        goto cleanup;

    return find_path_bfs(level);

cleanup:
    free_map(level);
    return status;
}

void free_map(Game *level) {
    if (!level->map)
        return;
    for (size_t y = 0; y < level->size.y; y++)
        free(level->map[y]);
    free(level->map);
}
