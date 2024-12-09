#include "Bolillo.h"
#include "GameLoopScene.h"
#include "audio/include/AudioEngine.h"

Bolillo::Bolillo()
    : _isFacingRight(true)
    , _speed(200.f)
    , _isMoving(false)
    , _currentAnimation("")
    , _mousePosition(cocos2d::Vec2::ZERO)
    , _handSprite(nullptr)
    , _life(100)
    , _healthBar(nullptr)
    , _anim(nullptr) {
}

Bolillo::~Bolillo() {
    if (_healthBar) {
        _healthBar->removeFromParent();
        _healthBar = nullptr;
    }

    if (_anim) {
        delete _anim;
        _anim = nullptr;
    }

    stopAllActions();
}

Bolillo* Bolillo::create() {
    Bolillo* ret = new Bolillo();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool Bolillo::init() {
    
    //Create main sprite
    auto bolilloSprite = Sprite::create();
    if (!bolilloSprite->initWithFile("BolilloW/Bolillo_0.png")) {
        return false;
    }
    setScale(0.4f);
    auto winSize = cocos2d::Director::getInstance()->getWinSize();
    setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
    
    //Set up velocity vector
    _velocity = cocos2d::Vec2::ZERO;
    
    //Create center pivot for right hand
    _centerNode = cocos2d::Node::create();
    _centerNode->setPosition(cocos2d::Vec2(180, 250));
    addChild(_centerNode);         
    
    //Create right hand
    _handSprite = Sprite::create("hand.png");
    _handSprite->setPosition(cocos2d::Vec2(0, 350));
    _handSprite->setScale(0.6f);
    _handSprite->setAnchorPoint(cocos2d::Vec2(0.3f, 0.5f));
    _centerNode->addChild(_handSprite);

    // Crear el fondo de la barra de vida
    auto healthBarBackground = cocos2d::Sprite::create("health_empty.png");
    healthBarBackground->setPosition(cocos2d::Vec2(225, 550));
    this->addChild(healthBarBackground, 0);  // Asegurarse de que esté detrás (z-order 0)

    // Crear la barra de vida
    auto healthBarForeground = cocos2d::Sprite::create("health_full.png");
    _healthBar = cocos2d::ProgressTimer::create(healthBarForeground);
    _healthBar->setType(cocos2d::ProgressTimer::Type::BAR);
    _healthBar->setMidpoint(cocos2d::Vec2(0, 0));  // La barra empieza desde la izquierda
    _healthBar->setBarChangeRate(cocos2d::Vec2(1, 0));  // Solo cambia horizontalmente
    _healthBar->setPercentage(100);  // Vida inicial en 100%
    _healthBar->setPosition(cocos2d::Vec2(225, 550));
    this->addChild(_healthBar, 1);  // Asegurarse de que esté delante (z-order 1)

    // Event listener for keyboard
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Bolillo::handleKeyPress, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Bolillo::handleKeyRelease, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    
    //Event listener for mouse
    auto mouseListener = cocos2d::EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(Bolillo::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    mouseListener->onMouseDown = CC_CALLBACK_1(Bolillo::onMouseDown, this);

  
    //Schedule update method
    scheduleUpdate();
    return true;
}

void Bolillo::update(float dt) {
    // Update sprite position
    auto newPosition = getPosition() + _velocity * dt;

    // Get the window sizeddddddd
    auto winSize = cocos2d::Director::getInstance()->getWinSize();

    // Restrict the position to be within the screen boundaries
    float clampedX = std::max(0.0f, std::min(newPosition.x, winSize.width - 200));
    float clampedY = std::max(0.0f, std::min(newPosition.y, winSize.height));
    setPosition(cocos2d::Vec2(clampedX, clampedY));

    // Calculate direction and angle to mouse
    auto worldPosition = getParent()->convertToWorldSpace(getPosition());
    auto direction = _mousePosition - worldPosition;
    float angle = CC_RADIANS_TO_DEGREES(atan2f(direction.y, direction.x));

    // Rotate the hand sprite to point towards the mouse
    if (_isFacingRight) {
        _handSprite->setRotation(-(110 - angle));
    }
    else {
        _handSprite->setRotation(-80 + angle);
    }

    // Rotate the center node to follow the mouse direction
    if (_centerNode) {
        _centerNode->setRotation(-(270 - angle));
    }

    // Change animation depending on movement
    if (_velocity != cocos2d::Vec2::ZERO) { // If there is movement
        if (_currentAnimation != "walk") {
            walkAnimation();
            _currentAnimation = "walk";
        }
    }
    else { // If there is no movement
        if (_currentAnimation != "idle") {
            idleAnimation();
            _currentAnimation = "idle";
        }
    }

    // Flip the sprites depending on the mouse position
    if (_mousePosition.x > getPositionX()) {
        if (!_isFacingRight) {
            _isFacingRight = true;
            setFlippedX(false);
            _handSprite->setFlippedX(false);
        }
    }
    else if (_mousePosition.x < getPositionX()) {
#include "audio/include/AudioEngine.h"
        if (_isFacingRight) {
            _isFacingRight = false;
            setFlippedX(true);
            _handSprite->setFlippedX(true);
        }
    }
}


void Bolillo::onMouseDown(cocos2d::EventMouse* event) {
    // Verifica si el clic es del botón derecho
    if (event->getMouseButton() == cocos2d::EventMouse::MouseButton::BUTTON_LEFT) {
        shoot(); // Llama al método para disparar
        cocos2d::AudioEngine::play2d("untitled.mp3", false);
    }
}

void Bolillo::reduceLife(int damage) {
    _life -= damage;
    if (_life < 0) {
        _life = 0;  // Evitamos que la vida sea negativa
    }

    // Actualizar la barra de vida cada vez que cambia la vida
    if (_healthBar) {
        _healthBar->setPercentage(static_cast<float>(_life));  // Ajusta el porcentaje de la barra de vida
    }

    // Detener cualquier acción previa relacionada con el cambio de color
    stopActionByTag(100); // 100 es un valor arbitrario para identificar esta acción

    // Cambiar color a rojo
    setColor(cocos2d::Color3B::RED);
    _handSprite->setColor(cocos2d::Color3B::RED);

    // Restaurar el color original después de 1 segundo
    auto delay = cocos2d::DelayTime::create(0.1f); // Duración del cambio
    auto restoreColor = cocos2d::CallFunc::create([this]() {
        setColor(cocos2d::Color3B::WHITE); // Restaurar el color original
        _handSprite->setColor(cocos2d::Color3B::WHITE);
    });

    // Crear una secuencia con la etiqueta
    auto sequence = cocos2d::Sequence::create(delay, restoreColor, nullptr);
    sequence->setTag(100); // Asignar una etiqueta para identificar esta acción

    // Ejecutar la secuencia
    runAction(sequence);
}

int Bolillo::getLife() {
    return _life;
}

void Bolillo::setLife(int life) {
    _life = life;
}

float Bolillo::getSpeed() {
    return _speed;
}

void Bolillo::setSpeed(float speed) {
    _speed = speed;
}

void Bolillo::onMouseMove(cocos2d::EventMouse* event) {
    _mousePosition = cocos2d::Vec2(event->getLocation().x, event->getLocation().y); //update mouse position 
}

void Bolillo::idleAnimation() { //Creation of idle animation
    auto idle = cocos2d::Animation::create();
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_00.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_01.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_02.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_03.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_04.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_05.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_06.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_07.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_08.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_09.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_10.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_11.png");
    idle->addSpriteFrameWithFile("BolilloIdle/Armature_idle_12.png");
    idle->setDelayPerUnit(0.04f);
    
    stopAllActions();
    runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(idle)));
}

void Bolillo::walkAnimation() { //Creation of walk animation
    auto animation = cocos2d::Animation::create();
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_0.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_1.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_2.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_3.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_4.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_5.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_6.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_7.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_8.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_9.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_10.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_11.png");
    animation->addSpriteFrameWithFile("BolilloW/Bolillo_12.png");
    animation->setDelayPerUnit(0.04f);
    
    stopAllActions();
    runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
}

void Bolillo::handleKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) { //Move the sprite with wasd keys
    switch (keyCode) {
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        _velocity.y = _speed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        _velocity.y = -_speed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        _velocity.x = -_speed;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        _velocity.x = _speed;
        break;
    default:
        break;
    }
}

void Bolillo::handleKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    switch (keyCode) { //Stop the character movement when key is released
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        _velocity.y = 0;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        _velocity.x = 0; 
        break;
    default:
        break;
    }
    _isMoving = (_velocity != cocos2d::Vec2::ZERO);
}

void Bolillo::shoot() {
    if (!_handSprite) return;

    // Convertir la posición de _handSprite al espacio global
    auto handWorldPosition = _handSprite->getParent()->convertToWorldSpace(_handSprite->getPosition());

    // Dirección hacia el mouse (del origen al destino)
    auto direction = (_mousePosition - handWorldPosition);
    direction.y = -direction.y;  // Explicitly flip the y component
    direction.normalize();

    // (Crea la bala como lo hiciste anteriormente)
    auto bullet = Bullet::create(direction);
    bullet->setPosition(handWorldPosition);

    // Agregar la bala al juego
    auto parent = dynamic_cast<GameLoopScene*>(this->getParent());
    if (parent) {
        parent->_bullets.pushBack(bullet); // Agrega la bala a la lista en GameLoopScene
    }

    getParent()->addChild(bullet);         // Agregar bala a la misma capa de Bolillo
}








