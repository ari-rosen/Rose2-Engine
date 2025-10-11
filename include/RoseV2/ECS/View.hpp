#pragma once

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/ECS/ComponentManager.hpp"
#include "RoseV2/ECS/ComponentArray.hpp"

namespace RoseV2::ECS {

class iComponentArray;

template<typename... Components>
class View {
public:
    class Iterator;

    View(ComponentManager *manager)
        :m_Manager(manager)
    {}

    Iterator begin() const {
        const iComponentArray* smallestArray = m_Manager->getSmallestArray<Components...>();
        
        const auto* map = smallestArray ? &smallestArray->getEntityToIndexMap() : nullptr;

        if (!map || map->empty()) {
            return end();
        }

        return Iterator(m_Manager, map->cbegin(), map->cend());
    }

    Iterator end() const {
        const iComponentArray* smallestArray = m_Manager->getSmallestArray<Components...>();
        const auto* map = smallestArray ? &smallestArray->getEntityToIndexMap() : nullptr;
        
        return Iterator(m_Manager, 
                        map ? map->cend() : typename Iterator::MapIterator(), 
                        map ? map->cend() : typename Iterator::MapIterator());
    }

    size_t size() const {
        return std::distance(begin(), end());
    }

    template<typename Func>
    requires std::regular_invocable<Func&, Components&...>
    void forEach(Func &&f) {
        for (auto e : *this) {
            std::forward<Func>(f)(m_Manager->getComponent<Components>(e)...);
        }
    }
private:
    ComponentManager *m_Manager;
};

// ====================================================================

template<typename... Components>
class View<Components...>::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Entity;
    
    using MapType = std::unordered_map<Entity, uint32_t, EntityHash>;
    using MapIterator = MapType::const_iterator;
public:
    Iterator(ComponentManager *manager, MapIterator current, MapIterator end)
        : m_Manager(manager), m_CurrentIt(current), m_EndIt(end)
    {
        if (m_CurrentIt != m_EndIt) {
            findNextValidEntity();
        }
    }

    Iterator& operator++() {
        if (m_CurrentIt != m_EndIt) {
            ++m_CurrentIt;
            findNextValidEntity();
        }
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    value_type operator*() {
        ROSEV2_ASSERT(m_CurrentIt != m_EndIt, "View iterator dereferenced out of range");
        return m_CurrentIt->first;
    }

    bool operator==(const Iterator &other) const {
        return m_CurrentIt == other.m_CurrentIt;
    }

    bool operator!=(const Iterator &other) const {
        return !(*this == other);
    }

private:
    void findNextValidEntity() {
        while (m_CurrentIt != m_EndIt) {
            value_type entity = m_CurrentIt->first;

            if (m_Manager->hasAllComponents<Components...>(entity)) {
                return;
            }
            ++m_CurrentIt;
        }
    }

private:
    ComponentManager *m_Manager = nullptr;
    MapIterator m_CurrentIt;
    MapIterator m_EndIt;
};

}
