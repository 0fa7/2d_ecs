#include "ECS.hpp"
#include "Logger/Logger.hpp"

Entity::Entity(int id) :
    m_id(id)
{
}

Entity::~Entity()
{
}

Entity::Entity(const Entity& other)
{
    *this = other;
}

int Entity::GetId() const
{
    return m_id;
}

bool Entity::operator==(const Entity& other) const
{
    return m_id == other.m_id;
}

Entity Entity::operator=(const Entity& other)
{
    m_id = other.m_id;
    return *this;
}

bool Entity::operator!=(const Entity& other) const
{
    return m_id != other.m_id;
}

bool Entity::operator>(const Entity& other) const
{
    return m_id > other.m_id;
}

bool Entity::operator<(const Entity& other) const
{
    return m_id < other.m_id;
}


System::System()
{
}

System::~System()
{
}

void System::AddEntityFromSystem(Entity entity)
{
    m_entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
    for(auto it = m_entities.begin(); it != m_entities.end(); it++)
    {
        if(*it == entity)
        {
            m_entities.erase(it);
            break;
        }
    }
    
}

std::vector<Entity> System::GetSystemEntities() const
{
    return m_entities;
}

const Signature& System::GetComponentSignature() const
{
    return m_component_signature;
};

Registry::Registry() :
    m_num_entities(0)
{
}

Registry::~Registry()
{
}

Entity Registry::CreateEntity()
{
    int entity_id = m_num_entities++;
    Entity entity(entity_id);
    m_entities_to_be_added.insert(entity);

    Logger::Info("Entity created: " + std::to_string(entity_id));

    return entity;
}

void Registry::Update()
{

}

void Registry::AddEntityToSystem(Entity entity)
{
}