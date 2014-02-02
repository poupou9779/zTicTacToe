#ifndef DEF_H
#define DEF_H

#include <SDL.h>

#define NUMBER_OF_TILES 3
#define HEIGHT_CELL 128
#define WIDTH_CELL 128

typedef short bool;
#define true 1
#define false 0

enum {FREE = 0, PLAYER1 = 1, PLAYER2 = 2};

typedef struct
{
    SDL_Surface *image;
    SDL_Rect pos[NUMBER_OF_TILES];
}
tileset_t;

typedef struct
{
    int board[3][3];
    SDL_Surface *screen;
    tileset_t tileset;
}
context_t;

void InitContext(context_t *ctxt);
void FreeContext(context_t *ctxt);

bool iscomplete(const int board[3][3]);
bool isrowOK(const int board[3][3], int row);
bool iscolumnOK(const int board[3][3], int column);
bool isdiagonaleOK(const int board[3][3]);
bool isover(const int board[3][3], bool *exaequo);

void Play(context_t *ctxt);
void Player1(context_t *ctxt);
void Player2(context_t *ctxt);

void BlitAll(context_t *ctxt);

#endif
