#include "Bullet.h"
#include "GameLoopScene.h"

Bullet* Bullet::create(const cocos2d::Vec2& direction) {
    Bullet* bullet = new Bullet();
    if (bullet && bullet->initWithFile("bullet.png")) {
        bullet->_direction = direction.getNormalized();
        bullet->_speed = 600.0f; // Velocidad de la bala
        bullet->_lifespan = 0.5f; // Duración en segundos que la bala existirá
        bullet->scheduleUpdate(); // Llama a update cada frame
        bullet->autorelease();
        bullet->setScale(0.3f);
        return bullet;
    }
    CC_SAFE_DELETE(bullet);
    return nullptr;
}

void Bullet::update(float dt) {
    // Reduce el tiempo de vida de la bala
    _lifespan -= dt;

    // Si la bala ha vivido lo suficiente, eliminarla
    if (_lifespan <= 0) {
        // Obtén el padre, que es la escena del juego
        auto parent = dynamic_cast<GameLoopScene*>(getParent());
        if (parent) {
            // Primero elimina la bala de la escena y limpia los recursos asociados
            removeFromParentAndCleanup(true);

            // Luego elimina la bala de la lista de balas
            parent->_bullets.eraseObject(this); // Esto elimina la bala de la lista
        }
        return;
    }

    // Mueve la bala en la dirección especificada
    setPosition(getPosition() + _direction * _speed * dt);
}

float Bullet::getLifeSpan() {
    return _lifespan;
}

void Bullet::setLifeSpan(float lifespan) {
    _lifespan += lifespan;
}
