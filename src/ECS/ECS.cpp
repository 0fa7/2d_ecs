#include "ECS.hpp"
#include "Logger/Logger.hpp"

int IComponent::m_next_id = 0;

Entity::Entity(int id) :
    m_id(id),
    m_registry(nullptr)
{
}

Entity::~Entity()
{
}

int Entity::GetId() const
{
    return m_id;
}

bool Entity::operator==(const Entity &other) const
{
    return m_id == other.m_id;
}

bool Entity::operator!=(const Entity &other) const
{
    return m_id != other.m_id;
}

bool Entity::operator>(const Entity &other) const
{
    return m_id > other.m_id;
}

bool Entity::operator<(const Entity &other) const
{
    return m_id < other.m_id;
}

bool Entity::operator<=(const Entity &other) const
{
    return m_id <= other.m_id;
}

void System::AddEntityToSystem(Entity entity)
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
    entity.m_registry = this;

    m_entities_to_be_added.insert(entity);
    
    // make sure entity component signatures vector can fit new entity
    if(entity_id >= m_entity_component_signatures.size())
    {
        m_entity_component_signatures.resize(entity_id + 1);
    }

    Logger::Info("Entity created: " + std::to_string(entity_id));

    return entity;
}

void Registry::Update()
{
    for(auto entity : m_entities_to_be_added)
    {
        AddEntityToSystems(entity);
    }

    m_entities_to_be_added.clear();
}

void Registry::AddEntityToSystems(Entity entity)
{
    const auto entity_id = entity.GetId();
    const auto entity_component_signature = m_entity_component_signatures[entity_id];

    for(auto &system : m_systems)
    {
        const auto &system_component_signature = system.second->GetComponentSignature();

        bool is_interested = (entity_component_signature & system_component_signature) == 
            system_component_signature;

        if(is_interested)
        {
            system.second->AddEntityToSystem(entity);
        }
    }
}