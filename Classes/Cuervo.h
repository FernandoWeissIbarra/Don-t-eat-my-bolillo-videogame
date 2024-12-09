#pragma once

#pragma once
#ifndef CUERVO_H
#define CUERVO_H
#include "cocos2d.h"
#include "physics/CCPhysicsBody.h"

// The Cuervo class represents an enemy character in the game.
// It inherits from cocos2d::Sprite and includes behavior for movement,
// animations, and health management.
class Cuervo : public cocos2d::Sprite {
public:
    Cuervo(); // Constructor: Initializes Cuervo's default attributes.
    ~Cuervo(); // Destructor: Cleans up resources, if needed.

    bool init(); // Initializes Cuervo's sprite and attributes.
    static Cuervo* create(); // Factory method to create a Cuervo object.

    void walkAnimation(); // Plays the walking animation.

    // Sets the target position (usually the player) for Cuervo to move towards.
    void setTargetPosition(const cocos2d::Vec2& targetPosition);

    // Updates Cuervo's position and behavior every frame.
    void update(float dt) override;

    // Spawns Cuervo at a random position on one of the screen's sides.
    void spawnAtRandomSide();

    // Reduces Cuervo's life points by a specified damage value.
    void reduceLife(int damage);

    // Returns the current life points of Cuervo.
    int getLife();

    void BossMode();

private:
    bool _isFacingRight; // Indicates if Cuervo is facing right.

    float _speed; // Movement speed of Cuervo.

    int _life; // Current life points of Cuervo.

    int damage; // Damage dealt by Cuervo.

    cocos2d::Animate* _anim; // Pointer to Cuervo's current animation.

    cocos2d::Vec2 _velocity; // Velocity vector for movement.

    cocos2d::Vec2 _targetPosition; // Position Cuervo is moving towards.

    cocos2d::ProgressTimer* _healthBar; // Pointer to Cuervo's health bar (used in Boss Mode).
};

#endif // CUERVO_H

