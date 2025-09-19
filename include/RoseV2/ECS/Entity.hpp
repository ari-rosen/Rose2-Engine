#pragma once

#include "RoseV2/pch.hpp"

namespace RoseV2::ECS {

struct Entity {
    uint32_t id;
    uint32_t generation;

    bool operator==(const Entity &other) const {
        return id == other.id && generation == other.generation;
    }

    static Entity Null() { return {UINT32_MAX, 0}; }
};

struct EntityHash {
    std::size_t operator()(const Entity& e) const noexcept {
        return std::hash<uint32_t>()(e.id) ^ (std::hash<uint32_t>()(e.generation) << 1);
    }
};

} // namespace RoseV2::ECS

