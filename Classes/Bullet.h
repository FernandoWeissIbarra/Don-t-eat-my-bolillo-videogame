#pragma once

#ifndef BULLET_H
#define BULLET_H

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite {
public:
    static Bullet* create(const cocos2d::Vec2& direction);

    void update(float dt);

    void  setLifeSpan(float lifespan);

    float getLifeSpan();

private:
    cocos2d::Vec2 _direction;  // Dirección en la que se mueve la bala
    float _speed;     // Velocidad de la bala
    float _lifespan;
};

#endif // BULLET_H