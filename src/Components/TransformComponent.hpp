#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
    TransformComponent(glm::vec2 position = glm::vec2{0.f, 0.f}, 
        glm::vec2 scale = glm::vec2{1.f, 1.f},
        double rotation = 0.0) :
        m_position(position),
        m_scale(scale),
        m_rotation(rotation)
    {

    }

    glm::vec2 m_position;
    glm::vec2 m_scale;
    double m_rotation;
};