#include <gtest/gtest.h>

#include "RoseV2/ECS/ComponentManager.hpp"
#include "RoseV2/ECS/Component.hpp"

using namespace RoseV2::ECS;

TEST(ECSComponentManagerTests, AddGetComponent) {
    ComponentManager manager;
    Entity e1 = 0;
    
    TransformComponent t{{10.0f, 20.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    manager.addComponent(e1, t);

    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e1));
    
    TransformComponent &got = manager.getComponent<TransformComponent>(e1);
    
    EXPECT_EQ(got.Translation.x, 10.0f);
    EXPECT_EQ(got.Translation.y, 20.0f);
    EXPECT_EQ(got.Translation.z, 0.0f);
    EXPECT_EQ(got.Rotation.x, 0.0f);
    EXPECT_EQ(got.Scale.x, 1.0f);
}

TEST(ECSComponentManagerTests, RemoveComponent) {
    ComponentManager manager;
    Entity e1 = 0;

    TransformComponent t{{5.0f, 15.0f, 0.0f}};
    manager.addComponent(e1, t);
    manager.removeComponent<TransformComponent>(e1);

    ASSERT_FALSE(manager.hasComponent<TransformComponent>(e1));
}

TEST(ECSComponentManagerTests, MultipleEntities) {
    ComponentManager manager;
    Entity e1 = 0;
    Entity e2 = 1;
    Entity e3 = 2;
    
    manager.addComponent<TransformComponent>(e1, {{10.0f, 10.0f, 0.0f}});
    manager.addComponent<TransformComponent>(e2, {{20.0f, 20.0f, 0.0f}});
    manager.addComponent<TransformComponent>(e3, {{30.0f, 30.0f, 0.0f}});
    
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e2));
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e3));
}

TEST(ECSComponentManagerTests, MultipleComponents) {
    ComponentManager manager;
    Entity e1 = 0;

    struct TestComponent {
        int32_t testData;
    };

    manager.addComponent<TransformComponent>(e1, {{100.0f, 100.0f, 0.0f}});
    manager.addComponent<TestComponent>(e1, {250});
    
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<TestComponent>(e1));
}

TEST(ECSComponentManagerTests, MultipleEntitiesComponentsRemove) {
    ComponentManager manager;

    Entity e1 = 0;
    Entity e2 = 1;
    Entity e3 = 2;

    struct Velocity {
        float vx, vy;
    };

    manager.addComponent<TransformComponent>(e1, {{10.0f, 20.0f, 0.0f}});
    manager.addComponent<Velocity>(e1, {1.0f, 2.0f});

    manager.addComponent<TransformComponent>(e2, {{30.0f, 40.0f, 0.0f}});
    manager.addComponent<Velocity>(e3, {5.0f, 6.0f});

    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<Velocity>(e1));
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e2));
    ASSERT_TRUE(manager.hasComponent<Velocity>(e3));

    manager.removeComponent<TransformComponent>(e1);
    manager.removeComponent<Velocity>(e3);

    ASSERT_FALSE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<Velocity>(e1));  // still exists
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e2));
    ASSERT_FALSE(manager.hasComponent<Velocity>(e3)); // removed
}
