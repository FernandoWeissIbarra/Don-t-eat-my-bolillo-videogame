#pragma once
#ifndef PATO_H
#define PATO_H
#include "cocos2d.h"
#include "physics/CCPhysicsBody.h"
#include "Enemy.h"

// The Pato class represents an enemy character in the game.
// It inherits from cocos2d::Sprite and includes behavior for movement,
// animations, and health management.
class Pato : public cocos2d::Sprite {
public:
    Pato(); // Constructor: Initializes Pato's default attributes.
    ~Pato(); // Destructor: Cleans up resources, if needed.

    bool init(); // Initializes Pato's sprite and attributes.
    static Pato* create(); // Factory method to create a Pato object.

    void walkAnimation(); // Plays the walking animation.

    // Sets the target position (usually the player) for Pato to move towards.
    void setTargetPosition(const cocos2d::Vec2& targetPosition);

    // Updates Pato's position and behavior every frame.
    void update(float dt) override;

    // Spawns Pato at a random position on one of the screen's sides.
    void spawnAtRandomSide();

    // Reduces Pato's life points by a specified damage value.
    void reduceLife(int damage);

    // Returns the current life points of Pato.
    int getLife();

    // Activates "Boss Mode," enhancing Pato's attributes and adding a health bar.
    void BossMode();

private:
    bool _isFacingRight; // Indicates if Pato is facing right.

    float _speed; // Movement speed of Pato.

    int _life; // Current life points of Pato.

    int damage; // Damage dealt by Pato.

    cocos2d::Animate* _anim; // Pointer to Pato's current animation.

    cocos2d::Vec2 _velocity; // Velocity vector for movement.

    cocos2d::Vec2 _targetPosition; // Position Pato is moving towards.

    cocos2d::ProgressTimer* _healthBar; // Pointer to Pato's health bar (used in Boss Mode).
};

#endif // PATO_H
