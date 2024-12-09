#pragma once

#pragma once
#ifndef PALOMA_H
#define PALOMA_H
#include "cocos2d.h"
#include "physics/CCPhysicsBody.h"

// The Paloma class represents an enemy character in the game.
// It inherits from cocos2d::Sprite and includes behavior for movement,
// animations, and health management.
class Paloma : public cocos2d::Sprite {
public:
    Paloma(); // Constructor: Initializes Paloma's default attributes.
    ~Paloma(); // Destructor: Cleans up resources, if needed.

    bool init(); // Initializes Paloma's sprite and attributes.
    static Paloma* create(); // Factory method to create a Paloma object.

    void walkAnimation(); // Plays the walking animation.

    // Sets the target position (usually the player) for Paloma to move towards.
    void setTargetPosition(const cocos2d::Vec2& targetPosition);

    // Updates Paloma's position and behavior every frame.
    void update(float dt) override;

    // Spawns Paloma at a random position on one of the screen's sides.
    void spawnAtRandomSide();

    // Reduces Paloma's life points by a specified damage value.
    void reduceLife(int damage);

    // Returns the current life points of Paloma.
    int getLife();

    void BossMode();

private:
    bool _isFacingRight; // Indicates if Paloma is facing right.

    float _speed; // Movement speed of Paloma.

    int _life; // Current life points of Paloma.

    int damage; // Damage dealt by Paloma.

    cocos2d::Animate* _anim; // Pointer to Paloma's current animation.

    cocos2d::Vec2 _velocity; // Velocity vector for movement.

    cocos2d::Vec2 _targetPosition; // Position Paloma is moving towards.

    cocos2d::ProgressTimer* _healthBar; // Pointer to Paloma's health bar (used in Boss Mode).
};

#endif // PALOMA_H

