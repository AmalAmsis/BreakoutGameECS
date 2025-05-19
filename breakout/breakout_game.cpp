/**
 * @file breakout_game.cpp
 * @brief Implementation of systems and entity creation functions using BAGEL ECS.
 *
 * This file contains the definitions of all systems and entity constructors
 * used in the Breakout-style game, implemented based on the ECS model with BAGEL.
 * All logic is placeholder only; the focus is on structure and mask-based iteration.
 */

#include "breakout_game.h"
#include "../bagel.h"
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

    bool isColliding(const Position& a, const Collider& ca, const Position& b, const Collider& cb) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float distSq = dx * dx + dy * dy;
        float radiusSum = ca.radius + cb.radius;
        return distSq <= radiusSum * radiusSum;
    }

    /**
     * @brief Detects and handles collisions between entities that have Position and Collider components.
     *        Also checks optional components like BallTag and BrickHealth for special behavior.
     */
    void CollisionSystem() {
        using namespace bagel;

        Mask mask;
        mask.set(Component<Position>::Bit);
        mask.set(Component<Collider>::Bit);

        for (id_type id1 = 0; id1 <= World::maxId().id; ++id1) {
            ent_type e1{id1};
            if (!World::mask(e1).test(mask)) continue;

            for (id_type id2 = id1 + 1; id2 <= World::maxId().id; ++id2) {
                ent_type e2{id2};
                if (!World::mask(e2).test(mask)) continue;

                auto& p1 = World::getComponent<Position>(e1);
                auto& c1 = World::getComponent<Collider>(e1);
                auto& p2 = World::getComponent<Position>(e2);
                auto& c2 = World::getComponent<Collider>(e2);

                if (!isColliding(p1, c1, p2, c2)) continue;

                bool ball1 = World::mask(e1).test(Component<BallTag>::Bit);
                bool ball2 = World::mask(e2).test(Component<BallTag>::Bit);

                // כדור פוגע בלבנה
                if (ball1 && World::mask(e2).test(Component<BrickHealth>::Bit)) {
                    auto& brick = World::getComponent<BrickHealth>(e2);
                    std::cout << "Ball hit brick! Remaining hits: " << brick.hits << "\n";
                    brick.hits--;
                    if (brick.hits <= 0) {
                        World::addComponent<DestroyedTag>(e2, {});
                    }
                }
                else if (ball2 && World::mask(e1).test(Component<BrickHealth>::Bit)) {
                    auto& brick = World::getComponent<BrickHealth>(e1);
                    brick.hits--;
                    if (brick.hits <= 0) {
                        World::addComponent<DestroyedTag>(e1, {});
                    }
                }

                // כדור פוגע בפדל — שנה את כיוון הכדור
                if (ball1 && World::mask(e2).test(Component<PaddleControl>::Bit)) {
                    std::cout << "Ball hit paddle! Inverting Y velocity.\n";

                    auto& vel = World::getComponent<Velocity>(e1);
                    vel.dy *= -1;
                }
                else if (ball2 && World::mask(e1).test(Component<PaddleControl>::Bit)) {
                    auto& vel = World::getComponent<Velocity>(e2);
                    vel.dy *= -1;
                }
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
    id_type CreateBall() {
        bagel::Entity e = bagel::Entity::create();
        e.addAll(Position{}, Velocity{}, Sprite{}, Collider{}, BallTag{});
        return e.entity().id;
    }

    /**
     * @brief Creates a brick entity with a specified amount of health.
     * @param health Number of hits required to break the brick
     * @return Unique entity ID
     */
    id_type CreateBrick(int health) {
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
    id_type CreatePaddle(int left, int right) {
        bagel::Entity e = bagel::Entity::create();
        e.addAll(Position{}, Sprite{}, Collider{}, PaddleControl{left, right});
        return e.entity().id;
    }

    /**
     * @brief Creates a falling power-up with a defined type and timed effect.
     * @param type Identifier for the power-up effect
     * @return Unique entity ID
     */
    id_type CreatePowerUp(int type) {
        bagel::Entity e = bagel::Entity::create();
        e.addAll(Position{}, Velocity{}, Sprite{}, Collider{}, PowerUpType{type}, TimedEffect{}, DestroyedTag{});
        return e.entity().id;
    }

    /**
     * @brief Creates a UI manager entity to display score and life count.
     * @return Unique entity ID
     */
    id_type CreateUIManager() {
        bagel::Entity e = bagel::Entity::create();
        e.addAll(Score{}, LifeCount{});
        return e.entity().id;
    }

    id_type CreateFloor() {
        bagel::Entity e = bagel::Entity::create();
        e.addAll(Position{400.0f, 590.0f}, Collider{1000}, FloorTag{});
        return e.entity().id;
    }
} // namespace breakout
