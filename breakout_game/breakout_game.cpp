/**
 * @file breakout_game.cpp
 * @brief Implementation of systems and entity creation functions using BAGEL ECS.
 *
 * This file contains the definitions of all systems and entity constructors
 * used in the Breakout-style game, implemented based on the ECS model with BAGEL.
 * All logic is placeholder only; the focus is on structure and mask-based iteration.
 */

#include "breakout_game.h"
#include <iostream>

namespace breakout {

//----------------------------------
// System Implementations
//----------------------------------

/**
 * @brief Updates positions of entities that have both Position and Velocity components.
 */
void MovementSystem() {
    bagel::Mask required;
    required.set(bagel::Component<Position>::Bit);
    required.set(bagel::Component<Velocity>::Bit);

    for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
        bagel::ent_type ent{id};
        if (bagel::World::mask(ent).test(required)) {
            // Movement logic would go here
        }
    }
}

/**
 * @brief Detects and handles collisions between entities that have Position and Collider components.
 *        Also checks optional components like BallTag and BrickHealth for special behavior.
 */
void CollisionSystem() {
    bagel::Mask required;
    required.set(bagel::Component<Position>::Bit);
    required.set(bagel::Component<Collider>::Bit);

    for (bagel::id_type id1 = 0; id1 <= bagel::World::maxId().id; ++id1) {
        bagel::ent_type e1{id1};
        if (!bagel::World::mask(e1).test(required)) continue;

        for (bagel::id_type id2 = 0; id2 <= bagel::World::maxId().id; ++id2) {
            if (id1 == id2) continue;
            bagel::ent_type e2{id2};
            if (!bagel::World::mask(e2).test(required)) continue;

            // Optional checks
            bool isBall = bagel::World::mask(e1).test(bagel::Component<BallTag>::Bit);
            bool hasHealth = bagel::World::mask(e2).test(bagel::Component<BrickHealth>::Bit);
            (void)isBall; (void)hasHealth;
        }
    }
}

/**
 * @brief Reads player input and updates paddle position accordingly.
 */
void PlayerControlSystem() {
    bagel::Mask required;
    required.set(bagel::Component<Position>::Bit);
    required.set(bagel::Component<PaddleControl>::Bit);

    for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
        bagel::ent_type ent{id};
        if (bagel::World::mask(ent).test(required)) {
            // Paddle movement based on input would go here
        }
    }
}

/**
 * @brief Applies power-up effects to entities, checking optional components like PaddleControl and BallTag.
 */
void PowerUpSystem() {
    bagel::Mask required;
    required.set(bagel::Component<PowerUpType>::Bit);
    required.set(bagel::Component<TimedEffect>::Bit);

    for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
        bagel::ent_type ent{id};
        if (bagel::World::mask(ent).test(required)) {
            bool hasPaddle = bagel::World::mask(ent).test(bagel::Component<PaddleControl>::Bit);
            bool hasBall = bagel::World::mask(ent).test(bagel::Component<BallTag>::Bit);
            (void)hasPaddle; (void)hasBall;
        }
    }
}

/**
 * @brief Removes entities that are marked for deletion using the DestroyedTag component.
 */
void DestroySystem() {
    bagel::Mask required;
    required.set(bagel::Component<DestroyedTag>::Bit);

    for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
        bagel::ent_type ent{id};
        if (bagel::World::mask(ent).test(required)) {
            // Destruction/removal logic would go here
        }
    }
}

/**
 * @brief Displays UI-related data such as score and lives for entities that hold those components.
 */
void UISystem() {
    bagel::Mask required;
    required.set(bagel::Component<Score>::Bit);
    required.set(bagel::Component<LifeCount>::Bit);

    for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
        bagel::ent_type ent{id};
        if (bagel::World::mask(ent).test(required)) {
            // Rendering of score/lives would go here
        }
    }
}

//----------------------------------
// Entity Creation Functions
//----------------------------------

/**
 * @brief Creates a new ball entity with basic motion and collision components.
 * @return Unique entity ID
 */
bagel::id_type CreateBall() {
    bagel::Entity e = bagel::Entity::create();
    e.addAll(Position{}, Velocity{}, Sprite{}, Collider{}, BallTag{});
    return e.entity().id;
}

/**
 * @brief Creates a brick entity with a specified amount of health.
 * @param health Number of hits required to break the brick
 * @return Unique entity ID
 */
bagel::id_type CreateBrick(int health) {
    bagel::Entity e = bagel::Entity::create();
    e.addAll(Position{}, Sprite{}, Collider{}, BrickHealth{health});
    return e.entity().id;
}

/**
 * @brief Creates a paddle entity with assigned movement key controls.
 * @param left Key code for left movement
 * @param right Key code for right movement
 * @return Unique entity ID
 */
bagel::id_type CreatePaddle(int left, int right) {
    bagel::Entity e = bagel::Entity::create();
    e.addAll(Position{}, Sprite{}, Collider{}, PaddleControl{left, right});
    return e.entity().id;
}

/**
 * @brief Creates a falling power-up with a defined type and timed effect.
 * @param type Identifier for the power-up effect
 * @return Unique entity ID
 */
bagel::id_type CreatePowerUp(int type) {
    bagel::Entity e = bagel::Entity::create();
    e.addAll(Position{}, Velocity{}, Sprite{}, Collider{}, PowerUpType{type}, TimedEffect{}, DestroyedTag{});
    return e.entity().id;
}

/**
 * @brief Creates a UI manager entity to display score and life count.
 * @return Unique entity ID
 */
bagel::id_type CreateUIManager() {
    bagel::Entity e = bagel::Entity::create();
    e.addAll(Score{}, LifeCount{});
    return e.entity().id;
}

} // namespace breakout
