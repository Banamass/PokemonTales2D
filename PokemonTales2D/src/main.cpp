#include "Game.h"

int main()
{
    Game game;

    srand(time(nullptr));

    while (!game.GetWindow()->IsDone()) {
        game.Update();
        game.Render();
        game.LateUpdate();
    }

    return 0;
}