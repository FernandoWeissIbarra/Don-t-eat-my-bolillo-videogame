#pragma once
#ifndef BOLILLO_H
#define BOLILLO_H
#include "cocos2d.h"
#include "physics/CCPhysicsBody.h"
#include "Bullet.h"
#include "audio/include/AudioEngine.h"

class Bolillo : public cocos2d::Sprite {
public:
    Bolillo();
    ~Bolillo();

    static Bolillo* create();

    virtual bool init() override;

    void walkAnimation();

    void idleAnimation();

    void onMouseMove(cocos2d::EventMouse* event);

    void reduceLife(int damage);

    int getLife();

    void setLife(int life);

    float getSpeed();

    void setSpeed(float speed);

    void handleKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void handleKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void update(float dt) override; // Actualiza la posición

    void shoot();

    void onMouseDown(cocos2d::EventMouse* event);
private:
    bool _isFacingRight;
    float _speed;
    int _life;
    int damage;
    cocos2d::Animate* _anim;
    cocos2d::Vec2 _velocity; // Vector de movimiento
    bool _isMoving;
    std::string _currentAnimation;
    cocos2d::Vec2 _mousePosition;
    cocos2d::Sprite* _handSprite;
    cocos2d::Node* _centerNode;
    cocos2d::ProgressTimer* _healthBar;
    cocos2d::Color3B _originalColor;
    bool _isReloading = false;
 };
#endif // BOLILLO_H

