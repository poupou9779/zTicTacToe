#include "def.h"
#include <stdio.h>

void InitContext(context_t *ctxt)
{
    const char *lTilesets[] = {"Tileset1.bmp", "Tileset2.bmp"};
    int i, j;
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Unable to load SDL ! %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);
    for(i = 0; i < 3; ++i)
        for(j = 0; j < 3; ++j)
            ctxt->board[i][j] = FREE;
    ctxt->screen = SDL_SetVideoMode(3*WIDTH_CELL, 3*HEIGHT_CELL, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    SDL_WM_SetCaption("zTicTacToe", NULL);
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
    if(ctxt->screen == NULL)
    {
        fprintf(stderr, "Unable to create the main surface : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    ctxt->tileset.image = SDL_LoadBMP(lTilesets[rand()%2]);
    if(ctxt->tileset.image == NULL)
    {
        fprintf(stderr, "Unable to load the tileset image ! %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < NUMBER_OF_TILES; ++i)
    {
        ctxt->tileset.pos[i].h = HEIGHT_CELL;
        ctxt->tileset.pos[i].w = WIDTH_CELL;
        ctxt->tileset.pos[i].x = i*WIDTH_CELL;
        ctxt->tileset.pos[i].y = 0;
    }
}

void FreeContext(context_t *ctxt)
{
    SDL_FreeSurface(ctxt->tileset.image);
    ctxt->tileset.image = NULL;
}

bool iscomplete(const int board[3][3])
{
    int i, j;
    for(i = 0; i < 3; ++i)
        for(j = 0; j < 3; ++j)
            if(board[i][j] == FREE)
                return false;
    return true;
}

bool isrowOK(const int board[3][3], int row)
{
    int i;
    int test = board[row][0];
    if(test == FREE)
        return false;
    for(i = 0; i < 3; ++i)
        if(board[row][i] != test)
            return false;
    return true;
}

bool iscolumnOK(const int board[3][3], int col)
{
    int i;
    int test = board[0][col];
    if(test == FREE)
        return false;
    for(i = 0; i < 3; ++i)
        if(board[i][col] != test)
            return false;
    return true;
}

bool isdiagonaleOK(const int board[3][3])
{
    bool ret = true;
    int i;
    int test = board[0][0];
    if(test == FREE)
        ret = false;
    if(ret)
    {
        for(i = 0; i < 3; ++i)
            if(board[i][i] != test)
                    ret = false;
    }
    if(ret)
        return true;
    test = board[0][2];
    if(test == FREE)
        return false;
    for(i = 0; i < 3; ++i)
        if(board[i][2-i] != test)
                return false;
    return true;
}

bool isover(const int board[3][3], bool *exaequo)
{
    int i;
    *exaequo = false;
    for(i = 0; i < 3; ++i)
        if(isrowOK(board, i) || iscolumnOK(board, i))
            return true;
    if(isdiagonaleOK(board))
        return true;
    *exaequo = true;
    if(iscomplete(board))
        return true;
    return false;
}

void Player1(context_t *ctxt)
{
    SDL_Event e;
    do
        SDL_WaitEvent(&e);
    while(e.type != SDL_MOUSEBUTTONDOWN || e.button.button != SDL_BUTTON_LEFT || ctxt->board[e.button.y/HEIGHT_CELL][e.button.x/WIDTH_CELL] != FREE);
    ctxt->board[e.button.y/HEIGHT_CELL][e.button.x/WIDTH_CELL] = PLAYER1;
}

void Player2(context_t *ctxt)
{
    int board_tmp[3][3];
    int x, y;
    bool exaequo;
    for(y = 0; y < 3; ++y)
        for(x = 0; x < 3; ++x)
            board_tmp[y][x] = ctxt->board[y][x];
    for(y = 0; y < 3; ++y)
    {
        for(x = 0; x < 3; ++x)
        {
            if(board_tmp[y][x] == FREE)
            {
                board_tmp[y][x] = PLAYER1;
                if(isover((const int(*)[3])board_tmp, &exaequo))
                {
                    ctxt->board[y][x] = PLAYER2;
                    return;
                }
                board_tmp[y][x] = PLAYER2;
                if(isover((const int(*)[3])board_tmp, &exaequo))
                {
                    ctxt->board[y][x] = PLAYER2;
                    return;
                }
                board_tmp[y][x] = FREE;
            }
        }
    }
    do
    {
        x = rand()%3;
        y = rand()%3;
    }
    while(ctxt->board[y][x] != FREE);
    ctxt->board[y][x] = PLAYER2;
}

void Play(context_t *ctxt)
{
    int lastplayer;
    bool exaequo;
    static char title[] = "Player 0 won !";
    while(!isover((const int(*)[3])ctxt->board, &exaequo))
    {
        BlitAll(ctxt);
        SDL_Flip(ctxt->screen);
        SDL_WM_SetCaption("Player 1", NULL);
        Player1(ctxt);
        lastplayer = 1;
        if(!isover((const int(*)[3])ctxt->board, &exaequo))
        {
            BlitAll(ctxt);
            SDL_Flip(ctxt->screen);
            SDL_WM_SetCaption("Player 2", NULL);
            SDL_Delay(500);
            Player2(ctxt);
            lastplayer = 2;
        }
        else
            break;
    }
    BlitAll(ctxt);
    SDL_Flip(ctxt->screen);
    if(!exaequo)
    {
        title[7] = lastplayer + '0';
        SDL_WM_SetCaption(title, NULL);
    }
    else
        SDL_WM_SetCaption("Exaequo !", NULL);
    SDL_Delay(2500);
}

void BlitAll(context_t *ctxt)
{
    int i, j;
    SDL_Rect dst;
    for(i = 0; i < 3; ++i)
    {
        dst.x = i*WIDTH_CELL;
        for(j = 0; j < 3; ++j)
        {
            dst.y = j*HEIGHT_CELL;
            SDL_BlitSurface(ctxt->tileset.image, &ctxt->tileset.pos[ctxt->board[j][i]], ctxt->screen, &dst);
        }
    }
}
