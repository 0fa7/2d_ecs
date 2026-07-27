#pragma once

#include <glm/glm.hpp>

struct SpriteComponent
{
    SpriteComponent(int width = 0, int height = 0) :
        m_width(width),
        m_height(height)
    {
    }

    int m_width;
    int m_height;
};