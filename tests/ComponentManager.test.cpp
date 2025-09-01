#include <gtest/gtest.h>

#include "RoseV2/ECS/ComponentManager.hpp"
#include "RoseV2/ECS/Component.hpp"

using namespace RoseV2::ECS;

TEST(ECSComponentManagerTests, AddGetComponent) {
    ComponentManager manager;
    Entity e1 = 0;
    
    Transform2D t{10, 20};
    manager.addComponent(e1, t);

    ASSERT_TRUE(manager.hasComponent<Transform2D>(e1));
    
    Transform2D &got = manager.getComponent<Transform2D>(e1);
    
    EXPECT_EQ(got.x, 10);
    EXPECT_EQ(got.y, 20);
}

TEST(ECSComponentManagerTests, RemoveComponent) {
    ComponentManager manager;
    Entity e1 = 0;

    Transform2D t{5, 15};
    manager.addComponent(e1, t);
    manager.removeComponent<Transform2D>(e1);
}

TEST(ECSComponentManagerTests, MultipleEntities) {
    ComponentManager manager;
    Entity e1 = 0;
    Entity e2 = 1;
    Entity e3 = 2;
    
    manager.addComponent<Transform2D>(e1, {10, 10});
    manager.addComponent<Transform2D>(e2, {10, 10});
    manager.addComponent<Transform2D>(e3, {10, 10});
    
    ASSERT_TRUE(manager.hasComponent<Transform2D>(e1));
    ASSERT_TRUE(manager.hasComponent<Transform2D>(e2));
    ASSERT_TRUE(manager.hasComponent<Transform2D>(e3));
}

TEST(ECSComponentManagerTests, MultipleComponents) {
    ComponentManager manager;
    Entity e1 = 0;

    struct TestComponent {
        int32_t testData;
    };

    manager.addComponent<Transform2D>(e1, {100, 100});
    manager.addComponent<TestComponent>(e1, {250});
    
    ASSERT_TRUE(manager.hasComponent<Transform2D>(e1));
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

    manager.addComponent<Transform2D>(e1, {10, 20});
    manager.addComponent<Velocity>(e1, {1.0f, 2.0f});

    manager.addComponent<Transform2D>(e2, {30, 40});
    manager.addComponent<Velocity>(e3, {5.0f, 6.0f});

    ASSERT_TRUE(manager.hasComponent<Transform2D>(e1));
    ASSERT_TRUE(manager.hasComponent<Velocity>(e1));
    ASSERT_TRUE(manager.hasComponent<Transform2D>(e2));
    ASSERT_TRUE(manager.hasComponent<Velocity>(e3));

    manager.removeComponent<Transform2D>(e1);
    manager.removeComponent<Velocity>(e3);

    ASSERT_FALSE(manager.hasComponent<Transform2D>(e1));
    ASSERT_TRUE(manager.hasComponent<Velocity>(e1));  // still exists
    ASSERT_TRUE(manager.hasComponent<Transform2D>(e2));
    ASSERT_FALSE(manager.hasComponent<Velocity>(e3)); // removed
}
