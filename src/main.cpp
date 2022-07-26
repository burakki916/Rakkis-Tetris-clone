#include "Game.h"
int main()
{
    Game newGame;
    while(!newGame.GetWindow()->IsDone()){
        newGame.handleInput();
        newGame.Update(); 
        newGame.Render(); 
    }
}

