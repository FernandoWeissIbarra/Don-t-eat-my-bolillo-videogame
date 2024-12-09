#pragma once

#pragma once
#ifndef BUITRE_H
#define BUITRE_H
#include "cocos2d.h"
#include "physics/CCPhysicsBody.h"

// The Buitre class represents an enemy character in the game.
// It inherits from cocos2d::Sprite and includes behavior for movement,
// animations, and health management.
class Buitre : public cocos2d::Sprite {
public:
    Buitre(); // Constructor: Initializes Buitre's default attributes.
    ~Buitre(); // Destructor: Cleans up resources, if needed.

    bool init(); // Initializes Buitre's sprite and attributes.
    static Buitre* create(); // Factory method to create a Buitre object.

    void walkAnimation(); // Plays the walking animation.

    // Sets the target position (usually the player) for Buitre to move towards.
    void setTargetPosition(const cocos2d::Vec2& targetPosition);

    // Updates Buitre's position and behavior every frame.
    void update(float dt) override;

    // Spawns Buitre at a random position on one of the screen's sides.
    void spawnAtRandomSide();

    // Reduces Buitre's life points by a specified damage value.
    void reduceLife(int damage);

    // Returns the current life points of Buitre.
    int getLife();

    void BossMode();

private:
    bool _isFacingRight; // Indicates if Buitre is facing right.

    float _speed; // Movement speed of Buitre.

    int _life; // Current life points of Buitre.

    int damage; // Damage dealt by Buitre.

    cocos2d::Animate* _anim; // Pointer to Buitre's current animation.

    cocos2d::Vec2 _velocity; // Velocity vector for movement.

    cocos2d::Vec2 _targetPosition; // Position Buitre is moving towards.

    cocos2d::ProgressTimer* _healthBar; // Pointer to Buitre's health bar (used in Boss Mode).
};

#endif // BUITRE_H

