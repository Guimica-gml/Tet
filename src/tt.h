#ifndef TT_H_
#define TT_H_

#include <stdlib.h>

#define TT_AT(tt, x, y) ((tt)->cells[(y) * (tt)->width + (x)])

typedef enum {
    TT_KIND_T,
    TT_KIND_L,
    TT_KIND_J,
    TT_KIND_S,
    TT_KIND_Z,
    TT_KIND_I,
    TT_KIND_O,
    TT_KIND_COUNT,
} TT_Kind;

typedef enum {
    TT_CELL_EMPTY,
    TT_CELL_FULL,
} TT_Cell_State;

typedef struct {
    TT_Cell_State state;
    TT_Kind kind;
} TT_Cell;

typedef enum {
    TT_FACE_DIR_UP,
    TT_FACE_DIR_RIGHT,
    TT_FACE_DIR_DOWN,
    TT_FACE_DIR_LEFT,
    TT_FACE_DIR_COUNT,
} TT_Face_Dir;

typedef struct {
    TT_Kind kind;
    TT_Cell_State *cells;
    TT_Face_Dir face_dir;
    size_t dim;
    size_t x;
    size_t y;
} TT_Tetro;

typedef struct {
    size_t top;
    size_t left;
    size_t bottom;
    size_t right;
} TT_Bbox;

typedef struct {
    size_t width;
    size_t height;
    TT_Cell *cells;
    TT_Tetro tetro;
} TT_Game;

TT_Game tt_game_new(size_t w, size_t h);
void tt_game_reset(TT_Game *game);
bool tt_game_tick(TT_Game *game);
void tt_game_free(TT_Game *game);

TT_Bbox tt_game_tetro_bbox(TT_Tetro *tetro);
void tt_game_tetro_set(TT_Game *game, TT_Kind kind);

void tt_game_tetro_rot_r(TT_Game *game);
void tt_game_tetro_rot_l(TT_Game *game);

void tt_game_tetro_mov_r(TT_Game *game);
void tt_game_tetro_mov_l(TT_Game *game);
void tt_game_tetro_mov_d(TT_Game *game);
void tt_game_tetro_jmp_d(TT_Game *game);

#endif // TT_H_
