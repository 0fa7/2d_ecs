#pragma once

#include "Logger/Logger.hpp"

#include <bitset>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
protected:
    static int m_next_id;
};

template <typename T>
class Component : public IComponent
{
public:
    Component() = default;
    ~Component() = default;

    static int GetId()
    {
        static auto id = m_next_id++;
        return id;
    }
};

class Entity
{
public:
    Entity(int id);
    ~Entity();
    Entity(const Entity &other) = default;

    int GetId() const;

    Entity& operator=(const Entity &other) = default;

    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;
    bool operator>(const Entity &other) const;
    bool operator<(const Entity &other) const;
    bool operator<=(const Entity &other) const;
    
    template <typename T, typename... TArgs>
    void AddComponent(TArgs &&...args);
    template <typename T>
    void RemoveComponent();
    template <typename T>
    bool HasComponent();
    template <typename T>
    T &GetComponent();

    class Registry* m_registry;

private:
    int m_id;
};

class System
{
public:
    System() = default;
    ~System() = default;
    void AddEntityToSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature &GetComponentSignature() const;
    template <typename TComponent>
    void RequireComponent();

private:
    Signature m_component_signature;
    std::vector<Entity> m_entities;
};

template <typename TComponent>
void System::RequireComponent()
{
    const auto component_id = Component<TComponent>::GetId();
    m_component_signature.set(component_id);
}

class IPool
{
public:
    virtual ~IPool()
    {
    }
};

template <typename T>
class Pool : public IPool
{
public:
    Pool(int size = 100)
    {
        m_data.resize(size);
    }

    virtual ~Pool() = default;

    bool IsEmpty()
    {
        return m_data.empty();
    }

    size_t GetSize()
    {
        return m_data.size();
    }

    void Resize(int size)
    {
        m_data.resize(size);
    }

    void Clear()
    {
        m_data.clear();
    }

    void Add(T object)
    {
        m_data.push_back(object);
    }

    void Set(int index, T object)
    {
        m_data[index] = object;
    }

    T &Get(int index)
    {
        return static_cast<T &>(m_data[index]);
    }

    T &operator[](unsigned int index)
    {
        return m_data[index];
    }

private:
    std::vector<T> m_data;
};

class Registry
{
public:
    Registry();
    ~Registry();
    void Update();
    Entity CreateEntity();

    // components
    template <typename T, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&...args);
    template <typename T>
    void RemoveComponent(Entity entity);
    template <typename T>
    bool HasComponent(Entity entity);
    template <typename T>
    T &GetComponent(Entity entity);

    // systems
    template <typename T, typename... TArgs>
    void AddSystem(TArgs... args);
    template <typename T>
    void RemoveSystem();
    template <typename T>
    bool HasSystem();
    template <typename T>
    T &GetSystem();

    // check component signature of entity and add to approriate systems
    void AddEntityToSystems(Entity entity);

//private:
    int m_num_entities;
    std::vector<std::shared_ptr<IPool>> m_component_pools;
    std::vector<Signature> m_entity_component_signatures;
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;
    std::set<Entity> m_entities_to_be_added;
    std::set<Entity> m_entities_to_be_killed;
};

template <typename T, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args)
{
    const auto component_id = Component<T>::GetId();
    const auto entity_id = entity.GetId();

    if (component_id >= m_component_pools.size())
    {
        m_component_pools.resize(component_id + 1, nullptr);
    }

    if (!m_component_pools[component_id])
    {   
        std::shared_ptr<Pool<T>> new_component_pool = std::make_shared<Pool<T>>();
        m_component_pools[component_id] = new_component_pool;
    }

    std::shared_ptr<Pool<T>> component_pool = std::static_pointer_cast<Pool<T>>(m_component_pools[component_id]);

    if (entity_id >= component_pool->GetSize())
    {
        component_pool->Resize(m_num_entities);
    }

    T new_component(std::forward<TArgs>(args)...);

    component_pool->Set(entity_id, new_component);
    m_entity_component_signatures[entity_id].set(component_id);

    Logger::Info("Component id " + std::to_string(component_id) + " added to " + std::to_string(entity_id));
}

template <typename T>
void Registry::RemoveComponent(Entity entity)
{
    const auto component_id = Component<T>::GetId();
    const auto entity_id = entity.GetId();
    m_entity_component_signatures[entity_id].set(component_id, false);
}

template <typename T>
bool Registry::HasComponent(Entity entity)
{
    const auto component_id = Component<T>::GetId();
    const auto entity_id = entity.GetId();
    return m_entity_component_signatures[entity_id].test(component_id);
}

template <typename T, typename... TArgs>
void Registry::AddSystem(TArgs... args)
{
    std::shared_ptr<T> system = std::make_shared<T>(std::forward<TArgs>(args)...);
    m_systems.insert(
        std::make_pair<std::type_index, std::shared_ptr<System>>(std::type_index(typeid(T)), system));
}

template <typename T>
void Registry::RemoveSystem()
{
    m_systems.erase(std::type_index(typeid(T)));
}

template <typename T>
bool Registry::HasSystem()
{
    return m_systems.find(std::type_index(typeid(T))) != m_systems.end();
}

template <typename T>
T &Registry::GetSystem()
{
    auto system = m_systems.find(std::type_index(typeid(T)));
    return *(std::static_pointer_cast<T>(system->second));
}

template <typename T>
T &Registry::GetComponent(Entity entity)
{
    const auto component_id = Component<T>::GetId();
    const auto entity_id = entity.GetId();
    auto component_pool = std::static_pointer_cast<Pool<T>>(m_component_pools[component_id]);
    return component_pool->Get(entity_id);
}

template<typename T, typename ...TArgs>
void Entity::AddComponent(TArgs &&...args)
{
    m_registry->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template <typename T>
void Entity::RemoveComponent()
{
    m_registry->RemoveComponent<T>(*this);
}

template <typename T>
bool Entity::HasComponent()
{
    return m_registry->HasComponent<T>(*this);
}

template <typename T>
T &Entity::GetComponent()
{
    return m_registry->GetComponent<T>(*this);
}