#include <SDL.h>
#include "def.h"
#include <time.h>

int main(int ac, char **av)
{
    context_t ctxt;
    srand((unsigned int)time(NULL));
    InitContext(&ctxt);
    Play(&ctxt);
    FreeContext(&ctxt);
    return 0;
    (void)ac;
    (void)av;
}
