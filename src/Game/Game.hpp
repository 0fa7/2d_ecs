#pragma once

class SDL_Renderer;
class SDL_Window;

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void Destroy();
    void ProcessInput();
    void Setup();
    void Update();
    void Render();

    int m_window_width;
    int m_window_height;

private:
    SDL_Renderer* m_renderer;
    SDL_Window* m_window;
    bool m_is_running;
    int millisecs_previous_frame = 0;
};