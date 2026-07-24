#include "Game/Game.hpp"
#include "Logger/Logger.hpp"

int main(int argc, char* argv[])
{
    Logger::Info("Hello world!");
    Logger::Err("Hello world!");
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();
}