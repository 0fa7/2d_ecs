#pragma once

class SDL_Renderer;
class SDL_Window;

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
    int m_offset = 0;
};