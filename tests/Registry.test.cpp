#include <gtest/gtest.h>

#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/ECS/Component.hpp"

#include <vector>
#include <algorithm>

using namespace RoseV2::ECS;

TEST(ECSRegistryTests, RemovingEntities) {
    Registry r;
    Entity e = r.createEntity();
    r.addComponent(e, TransformComponent{});
    r.addComponent(e, SpriteComponent{});

    Entity e2 = r.createEntity();
    r.addComponent(e2, TransformComponent{});
    r.addComponent(e2, SpriteComponent{});
    
    r.destroyEntity(e2);

    ASSERT_FALSE(r.isEntityAlive(e2));
    ASSERT_FALSE(r.hasComponent<TransformComponent>(e2));
    ASSERT_FALSE(r.hasComponent<SpriteComponent>(e2));
}

TEST(ECSRegistryTests, SingleEntityLifecycle) {
    Registry r;
    Entity e = r.createEntity();

    ASSERT_TRUE(r.isEntityAlive(e));
    ASSERT_FALSE(r.hasComponent<TransformComponent>(e));

    r.addComponent(e, TransformComponent{{1,2,3}});
    ASSERT_TRUE(r.hasComponent<TransformComponent>(e));
    auto& t = r.getComponent<TransformComponent>(e);
    EXPECT_EQ(t.Translation.x, 1);
    EXPECT_EQ(t.Translation.y, 2);
    EXPECT_EQ(t.Translation.z, 3);

    r.removeComponent<TransformComponent>(e);
    ASSERT_FALSE(r.hasComponent<TransformComponent>(e));

    r.destroyEntity(e);
    ASSERT_FALSE(r.isEntityAlive(e));
}

TEST(ECSRegistryTests, MultipleEntitiesAndComponents) {
    Registry r;
    const int N = 10;
    std::vector<Entity> entities;

    for (int i = 0; i < N; i++) {
        Entity e = r.createEntity();
        entities.push_back(e);

        r.addComponent(e, TransformComponent{{float(i), float(i*2), float(i*3)}});
        if (i % 2 == 0) {
            r.addComponent(e, SpriteComponent{{1,1,1,1}, 0});
        }
    }

    for (int i = 0; i < N; i++) {
        ASSERT_TRUE(r.hasComponent<TransformComponent>(entities[i]));
        if (i % 2 == 0) {
            ASSERT_TRUE(r.hasComponent<SpriteComponent>(entities[i]));
        } else {
            ASSERT_FALSE(r.hasComponent<SpriteComponent>(entities[i]));
        }
    }

    for (int i = 0; i < N; i += 3) {
        r.destroyEntity(entities[i]);
        ASSERT_FALSE(r.isEntityAlive(entities[i]));
        ASSERT_FALSE(r.hasComponent<TransformComponent>(entities[i]));
    }
}

TEST(ECSRegistryTests, RecycledEntityIDs) {
    Registry r;

    Entity e1 = r.createEntity();
    Entity e2 = r.createEntity();
    r.destroyEntity(e2);
    Entity e3 = r.createEntity();

    EXPECT_EQ(e2.id, e3.id);
    EXPECT_NE(e1.id, e3.id);

    ASSERT_FALSE(r.isEntityAlive({e2.id, e2.generation}));
    ASSERT_TRUE(r.isEntityAlive(e3));
}

TEST(ECSRegistryTests, ComponentsAfterDestroy) {
    Registry r;
    Entity e = r.createEntity();
    r.addComponent(e, TransformComponent{{1,2,3}});
    r.addComponent(e, SpriteComponent{{0.5f,0.5f,0.5f,1}, 1});

    r.destroyEntity(e);

    ASSERT_FALSE(r.isEntityAlive(e));
    ASSERT_FALSE(r.hasComponent<TransformComponent>(e));
    ASSERT_FALSE(r.hasComponent<SpriteComponent>(e));
}

TEST(ECSRegistryTests, StressTestEntities) {
    Registry r;
    const int N = 1000;
    std::vector<Entity> entities;

    for (int i = 0; i < N; i++) {
        Entity e = r.createEntity();
        entities.push_back(e);
        r.addComponent(e, TransformComponent{{float(i), float(i), float(i)}});
        r.addComponent(e, SpriteComponent{{1,1,1,1}, 0});
    }

    for (int i = 0; i < N; i += 3) {
        r.destroyEntity(entities[i]);
        ASSERT_FALSE(r.isEntityAlive(entities[i]));
    }

    for (int i = 0; i < N; i++) {
        if (i % 3 == 0) continue;
        ASSERT_TRUE(r.isEntityAlive(entities[i]));
        ASSERT_TRUE(r.hasComponent<TransformComponent>(entities[i]));
        ASSERT_TRUE(r.hasComponent<SpriteComponent>(entities[i]));
    }

    for (int i = 0; i < N/3; i++) {
        Entity newEntity = r.createEntity();
        ASSERT_TRUE(r.isEntityAlive(newEntity));
    }
}

TEST(ECSRegistryTests, AliveEntitiesComponentsCheck) {
    Registry registry;

    Entity e1 = registry.createEntity();
    Entity e2 = registry.createEntity();
    Entity e3 = registry.createEntity();
    
    registry.addComponent(e1, TransformComponent{ {1.0f, 2.0f, 3.0f} });
    registry.addComponent(e1, SpriteComponent{ {1.0f, 0.0f, 1.0f, 1.0f}, 42 });
    registry.addComponent(e2, TransformComponent{ {4.0f, 5.0f, 6.0f} });
    registry.addComponent(e2, SpriteComponent{ {0.5f, 0.5f, 0.5f, 1.0f} });
    registry.addComponent(e3, TransformComponent{ {7.0f, 8.0f, 9.0f} });
    registry.addComponent(e3, SpriteComponent{ {0.1f, 0.2f, 0.3f, 1.0f} });

    registry.destroyEntity(e2);
    
    auto alive = registry.view<TransformComponent, SpriteComponent>();

    ASSERT_EQ(alive.size(), 2);

    ASSERT_TRUE(std::find(alive.begin(), alive.end(), e2) == alive.end());

    for (auto e : alive) {
        ASSERT_TRUE(registry.isEntityAlive(e));

        auto& t = registry.getComponent<TransformComponent>(e);
        auto& s = registry.getComponent<SpriteComponent>(e);

        if (e == e1) {
            EXPECT_EQ(t.Translation.x, 1.0f);
            EXPECT_EQ(t.Translation.y, 2.0f);
            EXPECT_EQ(t.Translation.z, 3.0f);
            EXPECT_EQ(s.TextureID, 42);
            EXPECT_EQ(s.Color.r, 1.0f);
        } else if (e == e3) {
            EXPECT_EQ(t.Translation.x, 7.0f);
            EXPECT_EQ(t.Translation.y, 8.0f);
            EXPECT_EQ(t.Translation.z, 9.0f);
            EXPECT_EQ(s.Color.r, 0.1f);
        }
    }
}
