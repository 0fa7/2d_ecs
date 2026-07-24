#pragma once

#include <bitset>
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

template<typename T>
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
    Entity(const Entity& other);
    int GetId() const;
    
    Entity operator=(const Entity& other);
    bool operator==(const Entity& other) const;
    bool operator !=(const Entity& other) const;
    bool operator >(const Entity& other) const;
    bool operator <(const Entity& other) const;
private:
    int m_id;
};

class System
{
public:
    System();
    ~System();
    void AddEntityFromSystem(Entity entity);
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentSignature() const;
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

    ~Pool() = default;

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

    T& Get(int index)
    {
        return static_cast<T&>(m_data[index]);
    }

    T& operator[](unsigned int index)
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
    Entity CreateEntity();
    void Update();
    void AddEntityToSystem(Entity entity);
    template <typename T, typename ...TArgs>
    void AddComponent(Entity entity, TArgs&& ...args);
    template <typename T>
    void RemoveComponent(Entity entity);
    template <typename T>
    bool HasComponent(Entity entity);

private:
    int m_num_entities;
    std::vector<IPool*> m_component_pools;
    std::vector<Signature> m_entity_component_signatures;
    std::unordered_map<std::type_index, System*> m_systems;
    std::set<Entity> m_entities_to_be_added;
    std::set<Entity> m_entities_to_be_killed;
};

template <typename T, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
    const auto component_id = Component<T>::GetId();
    const auto entity_id = entity.GetId();

    if(component_id >= m_component_pools.size())
    {
        m_component_pools.resize(component_id + 1, nullptr);
    }

    if(!m_component_pools[component_id])
    {
        Pool<T>* new_component_pool = new Pool<T>;
        m_component_pools[component_id] = new_component_pool;
    }

    Pool<T>* component_pool = m_component_pools[component_id];

    if(entity_id >= component_pool->GetSize())
    {
        component_pool->Resize(m_num_entities);
    }

    T new_component(std::forward<TArgs>(args)...);

    component_pool->Set(entity_id, new_component);
    m_entity_component_signatures[entity_id].set(component_id);
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