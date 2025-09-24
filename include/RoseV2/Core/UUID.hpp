#pragma once

namespace RoseV2::Core {

class UUID {
public:
    UUID();
    UUID(uint64_t uuid);

    operator uint64_t() const { return m_UUID; }
private:
    uint64_t m_UUID;
};

}

namespace std {

template<>
struct hash<RoseV2::Core::UUID> {
    std::size_t operator()(const RoseV2::Core::UUID &uuid) const {
        return (uint64_t)uuid;
    }
};

}
