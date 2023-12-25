#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>
#include <time.h>

#include "./tt.h"

#define TT_WIDTH 16
#define TT_HEIGHT 20

#define CELL_SIZE 32

#define WINDOW_WIDTH (TT_WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (TT_HEIGHT * CELL_SIZE)

// In frames
#define TICK_INTERVAL 15

#define COLOR_BACKGROUND (Color) { 0x18, 0x18, 0x18, 0xFF }

typedef enum {
    GAME_STATUS_ONGOING,
    GAME_STATUS_GAME_OVER,
} Game_Status;

Color tetro_color(TT_Kind kind) {
    switch (kind) {
        case TT_KIND_T: return PURPLE;
        case TT_KIND_L: return ORANGE;
        case TT_KIND_J: return DARKBLUE;
        case TT_KIND_S: return GREEN;
        case TT_KIND_Z: return RED;
        case TT_KIND_I: return SKYBLUE;
        case TT_KIND_O: return YELLOW;
        default: {
            assert(0 && "unreachable");
        }
    }
}

void DrawCell(size_t x, size_t y, int cell_size, Color color) {
    DrawRectangle(x * cell_size, y * cell_size, cell_size, cell_size, color);
}

void DrawTetromino(size_t x, size_t y, TT_Tetro tetro, int cell_size) {
    Color color = tetro_color(tetro.kind);
    for (size_t ty = 0; ty < tetro.dim; ++ty) {
        for (size_t tx = 0; tx < tetro.dim; ++tx) {
            if (tetro.cells[ty * tetro.dim + tx] == TT_CELL_EMPTY) {
                continue;
            }
            DrawCell(x + tx, y + ty, cell_size, color);
        }
    }
}

void DrawTextCenter(const char *text, int x, int y, int font_size, float spacing, Color color) {
    Font font = GetFontDefault();
    Vector2 text_size = MeasureTextEx(font, text, font_size, spacing);
    Vector2 pos =  { x - text_size.x / 2, y - text_size.y / 2 };
    DrawTextEx(font, text, pos, font_size, spacing, color);
}

int main(void) {
    srand(time(NULL));

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tét");
    SetTargetFPS(60);

    TT_Game game = tt_game_new(TT_WIDTH, TT_HEIGHT);

    Game_Status status = GAME_STATUS_ONGOING;
    int timer = TICK_INTERVAL;

    while (!WindowShouldClose()) {
        if (status == GAME_STATUS_ONGOING) {
            if (IsKeyPressed(KEY_X))     tt_game_tetro_rot_l(&game);
            if (IsKeyPressed(KEY_C))     tt_game_tetro_rot_r(&game);
            if (IsKeyPressed(KEY_RIGHT)) tt_game_tetro_mov_r(&game);
            if (IsKeyPressed(KEY_LEFT))  tt_game_tetro_mov_l(&game);
            if (IsKeyPressed(KEY_DOWN))  tt_game_tetro_mov_d(&game);
            if (IsKeyPressed(KEY_SPACE)) tt_game_tetro_jmp_d(&game);

            timer -= 1;
            if (timer <= 0) {
                status = tt_game_tick(&game);
                timer = TICK_INTERVAL;
            }
        } else {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                tt_game_reset(&game);
                status = GAME_STATUS_ONGOING;
            }
        }

        BeginDrawing();
        ClearBackground(COLOR_BACKGROUND);

        for (size_t y = 0; y < game.height; ++y) {
            for (size_t x = 0; x < game.width; ++x) {
                if (TT_AT(&game, x, y).state == TT_CELL_EMPTY) {
                    continue;
                }
                Color color = tetro_color(TT_AT(&game, x, y).kind);
                DrawCell(x, y, CELL_SIZE, color);
            }
        }
        DrawTetromino(game.tetro.x, game.tetro.y, game.tetro, CELL_SIZE);

        if (status == GAME_STATUS_GAME_OVER) {
            DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, (Color) { 0x00, 0x00, 0x00, 0xCC });
            DrawTextCenter("Game Over", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 64, 8.0, WHITE);
            DrawTextCenter("(Press enter to restart)", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50, 20, 4.0, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    tt_game_free(&game);

    return 0;
}
