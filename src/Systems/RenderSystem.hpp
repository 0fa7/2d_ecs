#pragma once

#include "Components/SpriteComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "ECS/ECS.hpp"

#include <SDL2/SDL.h>

class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<SpriteComponent>();
        RequireComponent<TransformComponent>();
    }

    ~RenderSystem() = default;

    void Update(SDL_Renderer* renderer)
    {
        for(auto entity : GetSystemEntities())
        {     
            const auto& sprite = entity.GetComponent<SpriteComponent>();
            const auto& transform = entity.GetComponent<TransformComponent>();
            
            SDL_Rect obj_rect(
                static_cast<int>(transform.m_position.x),
                static_cast<int>(transform.m_position.y),
                sprite.m_width,
                sprite.m_height
            );
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            SDL_RenderFillRect(renderer, &obj_rect);
        }
    }
};