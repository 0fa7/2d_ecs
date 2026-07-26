#include "Game/Game.hpp"
#include "Logger/Logger.hpp"

int main(int argc, char* argv[])
{
    Logger::Info("2d_ecs!");
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();
}