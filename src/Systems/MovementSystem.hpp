#pragma once

#include "Components/RigidBodyComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "ECS/ECS.hpp"

class MovementSystem : public System
{
public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    ~MovementSystem() = default;

    void Update(double delta_time)
    {
        for(auto entity : GetSystemEntities())
        {     
            auto& transform = entity.GetComponent<TransformComponent>();
            const auto& rigid_body = entity.GetComponent<RigidBodyComponent>();

            transform.m_position.x += rigid_body.m_velocity.x * delta_time;
            transform.m_position.y += rigid_body.m_velocity.y * delta_time;

            Logger::Info("Entity id: " + std::to_string(entity.GetId()) + 
                " " + 
                std::to_string(transform.m_position.x) + 
                " " + 
                std::to_string(transform.m_position.y));
        }
    }
};