#include "Game.hpp"
#include "ECS/ECS.hpp"
#include "Logger/Logger.hpp"

#include <SDL2/SDL.h>

Game::Game() :
    m_window_width(2560),
    m_window_height(1440),
    m_window(nullptr),
    m_renderer(nullptr),
    m_is_running(false)
{
}

Game::~Game()
{
}

void Game::Initialize()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Failed to initialize SDL");
    }

    m_window = SDL_CreateWindow(
        "2d_ecs", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        m_window_width, 
        m_window_height, 
        0);

    if(!m_window)
    {
        Logger::Err("Failed to create window");
        m_is_running = false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        0);

    if(!m_renderer)
    {
        Logger::Err("Failed to create renderer");
        m_is_running = false;
    }

    millisecs_previous_frame = SDL_GetTicks();

    m_is_running = true;
}

void Game::Run()
{
    Setup();

    while(m_is_running)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Game::ProcessInput()
{
    SDL_Event sdl_event;

    while(SDL_PollEvent(&sdl_event))
    {
        switch(sdl_event.type)
        {
            case SDL_QUIT:
            {
                m_is_running = false;
                break;
            }
            case SDL_KEYDOWN:
            {
                break;
            }
            default:
            {
                break;
            }
                
        }
    }
}

void Game::Setup()
{
    
}

void Game::Update()
{
    // cap frame rate
    int time_to_wait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecs_previous_frame);

    if(time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME)
    {
        SDL_Delay(static_cast<Uint32>(time_to_wait));
    }
    
    //  calc delta time to use for consistant movement
    double delta_time = (SDL_GetTicks() - millisecs_previous_frame) / 1000.0;

    millisecs_previous_frame = SDL_GetTicks();
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
}