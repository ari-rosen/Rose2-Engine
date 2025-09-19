#include <gtest/gtest.h>

#include "RoseV2/ECS/LifetimeManager.hpp"
#include "RoseV2/ECS/Entity.hpp"

using namespace RoseV2::ECS;

TEST(ECSLifetimeManagerTests, CreateEntity) {
    LifetimeManager manager;
    Entity e = manager.createEntity();
    ASSERT_TRUE(manager.isEntityAlive(e));
}

TEST(ECSLifetimeManagerTests, CreateAndDestroy) {
    LifetimeManager manager;
    Entity e = manager.createEntity();
    ASSERT_TRUE(manager.isEntityAlive(e));
    
    manager.destroyEntity(e);
    ASSERT_TRUE(!manager.isEntityAlive(e));
}

TEST(ECSLifetimeManagerTests, CreateAndDestroyMultipleEntities) {
    LifetimeManager manager;
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    Entity e3 = manager.createEntity();
    
    ASSERT_TRUE(manager.isEntityAlive(e1));
    ASSERT_TRUE(manager.isEntityAlive(e2));
    ASSERT_TRUE(manager.isEntityAlive(e3));

    manager.destroyEntity(e2);

    ASSERT_FALSE(manager.isEntityAlive(e2));

    Entity e4 = manager.createEntity();
    ASSERT_TRUE(manager.isEntityAlive(e4));
}

TEST(ECSLifetimeManagerTests, RecycledIDs) {
    LifetimeManager manager;
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    manager.destroyEntity(e2);
    Entity e3 = manager.createEntity();
    ASSERT_EQ(e2.id, e3.id);
    ASSERT_NE(e2.generation, e3.generation);
}
