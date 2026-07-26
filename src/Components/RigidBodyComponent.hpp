#pragma once

#include <glm/glm.hpp>

struct RigidBodyComponent
{
    RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.f, 0.f)) :
        m_velocity(velocity)
    {
    }

    glm::vec2 m_velocity;
};