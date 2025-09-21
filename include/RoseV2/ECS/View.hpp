#pragma once

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/ECS/ComponentManager.hpp"

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
        if (!smallestArray || smallestArray->size() == 0) {
            return end();
        }
        return Iterator(m_Manager, 0, smallestArray);
    }
    Iterator end() const {
        const iComponentArray* smallestArray = m_Manager->getSmallestArray<Components...>();
        size_t endIndex = (smallestArray ? smallestArray->size() : 0);
        return Iterator(m_Manager, endIndex, smallestArray);
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

template<typename... Components>
class View<Components...>::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Entity;
    using pointer           = Entity*;
    using reference         = Entity&;
public:
    Iterator(ComponentManager *manager, size_t startIndex, const iComponentArray *smallestArray)
        : m_Manager(manager), m_CurrentIndex(startIndex), m_SmallestArray(smallestArray)
    {
        if (m_Manager && m_SmallestArray) {
            findNextValidEntity();
        }
    }

    Iterator& operator++() {
        if (m_SmallestArray) {
            m_CurrentIndex++;
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
        ROSEV2_ASSERT(!m_SmallestArray || m_CurrentIndex < m_SmallestArray->size(), "View iterator dereferenced out of range");
        return m_SmallestArray->getOwner(m_CurrentIndex);
    }

    bool operator==(const Iterator &other) {
        return m_CurrentIndex == other.m_CurrentIndex
            && m_SmallestArray == other.m_SmallestArray
            && m_Manager == other.m_Manager;
    }

    bool operator!=(const Iterator &other) {
        return !(*this == other);
    }

private:
    void findNextValidEntity() {
        if (!m_SmallestArray) return;

        while (m_CurrentIndex < m_SmallestArray->size()) {
            value_type entity = m_SmallestArray->getOwner(m_CurrentIndex);
            if (m_Manager->hasAllComponents<Components...>(entity)) {
                return;
            }
            ++m_CurrentIndex;
        }
    }

private:
    ComponentManager *m_Manager = nullptr;
    const iComponentArray* m_SmallestArray = nullptr;
    size_t m_CurrentIndex = 0;
};

}
