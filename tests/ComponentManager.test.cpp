#include <gtest/gtest.h>

#include "RoseV2/ECS/ComponentManager.hpp"
#include "RoseV2/ECS/Component.hpp"

using namespace RoseV2::ECS;

TEST(ECSComponentManagerTests, AddGetComponent) {
    ComponentManager manager;
    Entity e1 = {0,0};
    
    TransformComponent t{glm::vec3(10.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)};
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
    Entity e1 = {0,0};

    TransformComponent t{glm::vec3(5.0f, 15.0f, 0.0f)};
    manager.addComponent(e1, t);
    manager.removeComponent<TransformComponent>(e1);

    ASSERT_FALSE(manager.hasComponent<TransformComponent>(e1));
}

TEST(ECSComponentManagerTests, MultipleEntities) {
    ComponentManager manager;
    Entity e1 = {0,0};
    Entity e2 = {1,0};
    Entity e3 = {2,0};
    
    manager.addComponent<TransformComponent>(e1, {glm::vec3(10.0f, 10.0f, 0.0f)});
    manager.addComponent<TransformComponent>(e2, {glm::vec3(20.0f, 20.0f, 0.0f)});
    manager.addComponent<TransformComponent>(e3, {glm::vec3(30.0f, 30.0f, 0.0f)});
    
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e2));
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e3));
}

TEST(ECSComponentManagerTests, MultipleComponents) {
    ComponentManager manager;
    Entity e1 = {0,0};

    manager.addComponent<TransformComponent>(e1, {glm::vec3(100.0f, 100.0f, 0.0f)});
    manager.addComponent<SpriteComponent>(e1, {});
    
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<SpriteComponent>(e1));
}

TEST(ECSComponentManagerTests, MultipleEntitiesComponentsRemove) {
    ComponentManager manager;

    Entity e1 = {0,0};
    Entity e2 = {1,0};
    Entity e3 = {2,0};

    manager.addComponent<TransformComponent>(e1, {glm::vec3(10.0f, 20.0f, 0.0f)});
    manager.addComponent<SpriteComponent>(e1, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)});

    manager.addComponent<TransformComponent>(e2, {glm::vec3(30.0f, 40.0f, 0.0f)});
    manager.addComponent<SpriteComponent>(e3, {glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)});

    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<SpriteComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e2));
    ASSERT_TRUE(manager.hasComponent<SpriteComponent>(e3));

    manager.removeComponent<TransformComponent>(e1);
    manager.removeComponent<SpriteComponent>(e3);

    ASSERT_FALSE(manager.hasComponent<TransformComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<SpriteComponent>(e1));
    ASSERT_TRUE(manager.hasComponent<TransformComponent>(e2));
    ASSERT_FALSE(manager.hasComponent<SpriteComponent>(e3));
}

TEST(ECSComponentManagerTests, GetSmallestArray) {
    ComponentManager manager;

    Entity e1{0,0};
    Entity e2{1,0};
    Entity e3{2,0};
    Entity e4{3,0};

    manager.addComponent<TransformComponent>(e1, {glm::vec3(1,1,1)});
    manager.addComponent<TransformComponent>(e2, {glm::vec3(2,2,2)});
    manager.addComponent<TransformComponent>(e3, {glm::vec3(3,3,3)});

    manager.addComponent<SpriteComponent>(e1, {});
    manager.addComponent<SpriteComponent>(e2, {});

    manager.addComponent<TagComponent>(e1, {});

    const iComponentArray* smallest = manager.getSmallestArray<TransformComponent, SpriteComponent, TagComponent>();
    ASSERT_NE(smallest, nullptr);

    const ComponentArray<TagComponent>* tagArray = dynamic_cast<const ComponentArray<TagComponent>*>(smallest);
    ASSERT_NE(tagArray, nullptr);
    EXPECT_EQ(tagArray->size(), 1);

    const iComponentArray* smallest2 = manager.getSmallestArray<TransformComponent, SpriteComponent>();
    const ComponentArray<SpriteComponent>* spriteArray = dynamic_cast<const ComponentArray<SpriteComponent>*>(smallest2);
    ASSERT_NE(spriteArray, nullptr);
    EXPECT_EQ(spriteArray->size(), 2);
}
