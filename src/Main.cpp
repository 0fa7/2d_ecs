#include <iostream>

#include "Game.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello world!" << std::endl;
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();
}