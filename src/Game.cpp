#include "Game.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Game::Game() :
    m_window_width(2560),
    m_window_height(1440),
    m_window(nullptr),
    m_renderer(nullptr),
    m_is_running(true)
{
}

Game::~Game()
{
}

void Game::Initialize()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Failed to initialize SDL" << std::endl;
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
        std::cerr << "Failed to create window" << std::endl;
        m_is_running = false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        0);

    if(!m_renderer)
    {
        std::cerr << "Failed to create renderer" << std::endl;
        m_is_running = false;
    }
}

void Game::Run()
{
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
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    //SDL_Surface* surface = IMG_Load("../assets/images/tank-tiger-right.png");
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    //SDL_FreeSurface(surface);
    //SDL_Rect dstRect = {10 + m_offset++, 10, 128, 128};
    //SDL_RenderCopy(m_renderer, texture, nullptr, &dstRect);
    
    SDL_RenderPresent(m_renderer);
}