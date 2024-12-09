#pragma once

#pragma once
#ifndef GABIOTA_H
#define GABIOTA_H
#include "cocos2d.h"
#include "physics/CCPhysicsBody.h"
#include "Enemy.h"

// The Gabiota class represents an enemy character in the game.
// It inherits from cocos2d::Sprite and includes behavior for movement,
// animations, and health management.
class Gabiota : public cocos2d::Sprite {
public:
    Gabiota(); // Constructor: Initializes Gabiota's default attributes.
    ~Gabiota(); // Destructor: Cleans up resources, if needed.

    bool init(); // Initializes Gabiota's sprite and attributes.
    static Gabiota* create(); // Factory method to create a Gabiota object.

    void walkAnimation(); // Plays the walking animation.

    // Sets the target position (usually the player) for Gabiota to move towards.
    void setTargetPosition(const cocos2d::Vec2& targetPosition);

    // Updates Gabiota's position and behavior every frame.
    void update(float dt) override;

    // Spawns Gabiota at a random position on one of the screen's sides.
    void spawnAtRandomSide();

    // Reduces Gabiota's life points by a specified damage value.
    void reduceLife(int damage);

    // Returns the current life points of Gabiota.
    int getLife();

    // Activates "Boss Mode," enhancing Gabiota's attributes and adding a health bar.
    void BossMode();

private:
    bool _isFacingRight; // Indicates if Gabiota is facing right.

    float _speed; // Movement speed of Gabiota.

    int _life; // Current life points of Gabiota.

    int damage; // Damage dealt by Gabiota.

    cocos2d::Animate* _anim; // Pointer to Gabiota's current animation.

    cocos2d::Vec2 _velocity; // Velocity vector for movement.

    cocos2d::Vec2 _targetPosition; // Position Gabiota is moving towards.

    cocos2d::ProgressTimer* _healthBar; // Pointer to Gabiota's health bar (used in Boss Mode).
};

#endif // GABIOTA_H

