#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./tt.h"

TT_Cell_State tt_cells_T[] = {
    0, 1, 0,
    1, 1, 1,
    0, 0, 0,
};

TT_Cell_State tt_cells_L[] = {
    0, 0, 1,
    1, 1, 1,
    0, 0, 0,
};

TT_Cell_State tt_cells_J[] = {
    1, 0, 0,
    1, 1, 1,
    0, 0, 0,
};

TT_Cell_State tt_cells_S[] = {
    0, 1, 1,
    1, 1, 0,
    0, 0, 0,
};

TT_Cell_State tt_cells_Z[] = {
    1, 1, 0,
    0, 1, 1,
    0, 0, 0,
};

TT_Cell_State tt_cells_I[] = {
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

TT_Cell_State tt_cells_O[] = {
    0, 0, 0, 0,
    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
};

TT_Tetro tt_tetro_defs[] = {
    [TT_KIND_T] = { .kind = TT_KIND_T, .cells = tt_cells_T, .dim = 3 },
    [TT_KIND_L] = { .kind = TT_KIND_L, .cells = tt_cells_L, .dim = 3 },
    [TT_KIND_J] = { .kind = TT_KIND_J, .cells = tt_cells_J, .dim = 3 },
    [TT_KIND_S] = { .kind = TT_KIND_S, .cells = tt_cells_S, .dim = 3 },
    [TT_KIND_Z] = { .kind = TT_KIND_Z, .cells = tt_cells_Z, .dim = 3 },
    [TT_KIND_I] = { .kind = TT_KIND_I, .cells = tt_cells_I, .dim = 4 },
    [TT_KIND_O] = { .kind = TT_KIND_O, .cells = tt_cells_O, .dim = 4 },
};

#define TT_WALL_KICK_OFFSET_COUNT 5
#define TT_WALL_KICK_DATA_LEN 8

typedef struct {
    int x;
    int y;
} TT_Wall_Kick_Offset;

typedef struct {
    TT_Face_Dir from_dir;
    TT_Face_Dir to_dir;
    TT_Wall_Kick_Offset tests[TT_WALL_KICK_OFFSET_COUNT];
} TT_Wall_Kick_Def;

TT_Wall_Kick_Def wall_kick_data[TT_WALL_KICK_DATA_LEN] = {
    {
        .from_dir = 0,
        .to_dir = 1,
        .tests = { {0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2} }
    },
    {
        .from_dir = 1,
        .to_dir = 0,
        .tests = { {0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2} }
    },
    {
        .from_dir = 1,
        .to_dir = 2,
        .tests = { {0, 0}, { 1, 0}, { 1, -1}, {0,  2}, { 1,  2} }
    },
    {
        .from_dir = 2,
        .to_dir = 1,
        .tests = { {0, 0}, {-1, 0}, {-1,  1}, {0, -2}, {-1, -2} }
    },
    {
        .from_dir = 2,
        .to_dir = 3,
        .tests = { {0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2} }
    },
    {
        .from_dir = 3,
        .to_dir = 2,
        .tests = { {0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2} }
    },
    {
        .from_dir = 3,
        .to_dir = 0,
        .tests = { {0, 0}, {-1, 0}, {-1, -1}, {0,  2}, {-1,  2} }
    },
    {
        .from_dir = 0,
        .to_dir = 3,
        .tests = { {0, 0}, { 1, 0}, { 1,  1}, {0, -2}, { 1, -2} }
    },
};

TT_Wall_Kick_Def wall_kick_data_I[TT_WALL_KICK_DATA_LEN] = {
    {
        .from_dir = 0,
        .to_dir = 1,
        .tests = { {0, 0}, {-2, 0}, { 1, 0}, {-2, -1}, { 1,  2} }
    },
    {
        .from_dir = 1,
        .to_dir = 0,
        .tests = { {0, 0}, { 2, 0}, {-1, 0}, { 2,  1}, {-1, -2} }
    },
    {
        .from_dir = 1,
        .to_dir = 2,
        .tests = { {0, 0}, {-1, 0}, { 2, 0}, {-1,  2}, { 2, -1} }
    },
    {
        .from_dir = 2,
        .to_dir = 1,
        .tests = { {0, 0}, { 1, 0}, {-2, 0}, { 1, -2}, {-2,  1} }
    },
    {
        .from_dir = 2,
        .to_dir = 3,
        .tests = { {0, 0}, { 2, 0}, {-1, 0}, { 2,  1}, {-1, -2} }
    },
    {
        .from_dir = 3,
        .to_dir = 2,
        .tests = { {0, 0}, {-2, 0}, { 1, 0}, {-2, -1}, { 1,  2} }
    },
    {
        .from_dir = 3,
        .to_dir = 0,
        .tests = { {0, 0}, { 1, 0}, {-2, 0}, { 1, -2}, {-2,  1} }
    },
    {
        .from_dir = 0,
        .to_dir = 3,
        .tests = { {0, 0}, {-1, 0}, { 2, 0}, {-1,  2}, { 2, -1} }
    },
};

TT_Game tt_game_new(size_t w, size_t h) {
    TT_Game game = {
        .width = w,
        .height = h,
        .cells = calloc(w * h, sizeof(TT_Cell)),
        .tetro = { 0 },
    };
    TT_Kind kind = rand() % TT_KIND_COUNT;
    tt_game_tetro_set(&game, kind);
    return game;
}

void tt_game_reset(TT_Game *game) {
    memset(game->cells, 0, game->width * game->height * sizeof(TT_Cell));
    TT_Kind kind = rand() % TT_KIND_COUNT;
    tt_game_tetro_set(game, kind);
}

bool tt_game_tetro_test_move(TT_Game *game, TT_Tetro *tetro, int dirx, int diry) {
    TT_Bbox bbox = tt_game_tetro_bbox(tetro);

    int b_top = bbox.top + diry;
    int b_left = bbox.left + dirx;
    size_t b_bottom = bbox.bottom + diry;
    size_t b_right = bbox.right + dirx;

    if (b_top < 0 || b_left < 0 || b_bottom > game->height || b_right > game->width) {
        return false;
    }

    for (size_t dy = 0; dy < tetro->dim; ++dy) {
        for (size_t dx = 0; dx < tetro->dim; ++dx) {
            size_t tx = tetro->x + dirx + dx;
            size_t ty = tetro->y + diry + dy;
            TT_Cell_State tetro_cell = tetro->cells[dy * tetro->dim + dx];
            TT_Cell_State board_cell = game->cells[ty * game->width + tx].state;
            if (tetro_cell == TT_CELL_FULL && board_cell == TT_CELL_FULL) {
                return false;
            }
        }
    }

    return true;
}

bool tt_game_is_line_full(TT_Game *game, size_t line) {
    assert(line < game->height);
    for (size_t x = 0; x < game->width; ++x) {
        if (game->cells[line * game->width + x].state == TT_CELL_EMPTY) {
            return false;
        }
    }
    return true;
}

bool tt_game_tick(TT_Game *game) {
    if (tt_game_tetro_test_move(game, &game->tetro, 0, 1)) {
        game->tetro.y += 1;
        return false;
    }

    TT_Tetro tetro = game->tetro;
    for (size_t dy = 0; dy < tetro.dim; ++dy) {
        for (size_t dx = 0; dx < tetro.dim; ++dx) {
            if (tetro.cells[dy * tetro.dim + dx] == TT_CELL_FULL) {
                TT_AT(game, tetro.x + dx, tetro.y + dy) = (TT_Cell) {
                    TT_CELL_FULL,
                    tetro.kind,
                };
            }
        }
    }

    size_t line = game->height - 1;
    while (line > 0) {
        if (tt_game_is_line_full(game, line)) {
            memmove(&game->cells[game->width], game->cells, game->width * line * sizeof(TT_Cell));
            memset(game->cells, 0, game->width * sizeof(TT_Cell));
        } else {
            line -= 1;
        }
    }

    TT_Kind kind = rand() % TT_KIND_COUNT;
    tt_game_tetro_set(game, kind);

    return !tt_game_tetro_test_move(game, &game->tetro, 0, 0);
}

void tt_game_tetro_set(TT_Game *game, TT_Kind kind) {
    TT_Tetro *tt_def = &tt_tetro_defs[kind];
    size_t tt_size = tt_def->dim * tt_def->dim * sizeof(TT_Cell_State);

    game->tetro.kind = kind;
    game->tetro.dim = tt_def->dim;
    game->tetro.cells = realloc(game->tetro.cells, tt_size);
    memcpy(game->tetro.cells, tt_def->cells, tt_size);

    assert(game->width > game->tetro.dim && "Tetris grid is too smol");
    game->tetro.x = (game->width - game->tetro.dim) / 2;
    game->tetro.y = 0;
}

const char *tt_face_dir_to_cstr(TT_Face_Dir face_dir) {
    switch (face_dir) {
        case TT_FACE_DIR_UP: return "TT_FACE_DIR_UP";
        case TT_FACE_DIR_DOWN: return "TT_FACE_DIR_DOWN";
        case TT_FACE_DIR_RIGHT: return "TT_FACE_DIR_RIGHT";
        case TT_FACE_DIR_LEFT: return "TT_FACE_DIR_LEFT";
        default: {
            assert(0 && "unreachable");
        }
    }
}

bool tt_game_tetro_wall_kick(TT_Game *game, TT_Tetro *tetro) {
    TT_Face_Dir from_dir = game->tetro.face_dir;
    TT_Face_Dir to_dir = tetro->face_dir;

    TT_Wall_Kick_Def* table;
    if (tetro->kind == TT_KIND_I) {
        table = wall_kick_data_I;
    } else {
        table = wall_kick_data;
    }

    for (size_t i = 0; i < TT_WALL_KICK_DATA_LEN; ++i) {
        TT_Wall_Kick_Def *def = &table[i];
        if (from_dir == def->from_dir && to_dir == def->to_dir) {
            for (size_t j = 0; j < TT_WALL_KICK_OFFSET_COUNT; ++j) {
                TT_Wall_Kick_Offset offset = def->tests[j];
                if (tt_game_tetro_test_move(game, tetro, offset.x, offset.y)) {
                    tetro->x += offset.x;
                    tetro->y += offset.y;
                    return true;
                }
            }
            break;
        }
    }

    return false;
}

void tt_game_tetro_rot_r(TT_Game *game) {
    size_t dim = game->tetro.dim;
    size_t tt_size = dim * dim * sizeof(TT_Cell_State);

    TT_Tetro tetro = game->tetro;
    tetro.face_dir = (game->tetro.face_dir + 1) % TT_FACE_DIR_COUNT;
    tetro.cells = malloc(tt_size);

    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            size_t tx = dim - y - 1;
            size_t ty = x;
            tetro.cells[ty * dim + tx] = game->tetro.cells[y * dim + x];
        }
    }

    if (tt_game_tetro_wall_kick(game, &tetro)) {
        game->tetro.face_dir = tetro.face_dir;
        game->tetro.x = tetro.x;
        game->tetro.y = tetro.y;
        memcpy(game->tetro.cells, tetro.cells, tt_size);
    }
    free(tetro.cells);
}

void tt_game_tetro_rot_l(TT_Game *game) {
    size_t dim = game->tetro.dim;
    size_t tt_size = dim * dim * sizeof(TT_Cell_State);

    TT_Tetro tetro = game->tetro;
    tetro.face_dir = (game->tetro.face_dir - 1) % TT_FACE_DIR_COUNT;
    tetro.cells = malloc(tt_size);

    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            size_t tx = y;
            size_t ty = dim - x - 1;
            tetro.cells[ty * dim + tx] = game->tetro.cells[y * dim + x];
        }
    }

    if (tt_game_tetro_wall_kick(game, &tetro)) {
        game->tetro.face_dir = tetro.face_dir;
        game->tetro.x = tetro.x;
        game->tetro.y = tetro.y;
        memcpy(game->tetro.cells, tetro.cells, tt_size);
    }
    free(tetro.cells);
}

void tt_game_tetro_mov_r(TT_Game *game) {
    if (tt_game_tetro_test_move(game, &game->tetro, 1, 0)) {
        game->tetro.x += 1;
    }
}

void tt_game_tetro_mov_l(TT_Game *game) {
    if (tt_game_tetro_test_move(game, &game->tetro, -1, 0)) {
        game->tetro.x -= 1;
    }
}

void tt_game_tetro_mov_d(TT_Game *game) {
    if (tt_game_tetro_test_move(game, &game->tetro, 0, 1)) {
        game->tetro.y += 1;
    }
}

void tt_game_tetro_jmp_d(TT_Game *game) {
    while (tt_game_tetro_test_move(game, &game->tetro, 0, 1)) {
        game->tetro.y += 1;
    }
    tt_game_tick(game);
}

TT_Bbox tt_game_tetro_bbox(TT_Tetro *tetro) {
    TT_Bbox bbox = { tetro->dim, tetro->dim, 0, 0 };

    for (size_t dy = 0; dy < tetro->dim; ++dy) {
        for (size_t dx = 0; dx < tetro->dim; ++dx) {
            TT_Cell_State state = tetro->cells[dy * tetro->dim + dx];
            if (state == TT_CELL_EMPTY) {
                continue;
            }
            if (dx < bbox.left) bbox.left = dx;
            if (dy < bbox.top) bbox.top = dy;
            if (dx > bbox.right) bbox.right = dx;
            if (dy > bbox.bottom) bbox.bottom = dy;
        }
    }

    bbox.left += tetro->x;
    bbox.top += tetro->y;
    bbox.right += tetro->x + 1;
    bbox.bottom += tetro->y + 1;

    return bbox;
}

void tt_game_free(TT_Game *game) {
    free(game->cells);
    free(game->tetro.cells);
}
