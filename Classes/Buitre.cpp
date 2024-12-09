#include "Buitre.h"
#include "GameLoopScene.h"

// Constructor
Buitre::Buitre()
    : _isFacingRight(true) // Indicates if the sprite is facing right
    , _speed(70.f)        // Default movement speed
    , _life(10)             // Initial life points
    , damage(1)            // Damage dealt by Buitre
    , _anim(nullptr)       // Pointer for animation
    , _healthBar(nullptr)  // Pointer for the health bar, initialized to nullptr
{
}

// Destructor
Buitre::~Buitre() {
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

// Factory method to create a Buitre object
Buitre* Buitre::create() {
    Buitre* buitre = new Buitre();
    if (buitre && buitre->init()) {
        buitre->autorelease();
        return buitre;
    }
    else {
        delete buitre;
        return nullptr;
    }
}

// Reduces Buitre's life points by a specified damage value
void Buitre::reduceLife(int damage) {
    _life -= damage;

    if (_life < 0) {
        _life = 0; // Prevent life from going negative
    }

    // Update the health bar, if it exists
    if (_healthBar) {
        _healthBar->setPercentage(static_cast<float>(_life) / 50.0f * 100.0f);
    }

    // Stop any previous color change action
    stopActionByTag(100); // 100 is an arbitrary value to identify this action

    // Change color to red to indicate damage
    setColor(cocos2d::Color3B::RED);

    // Restore the original color after 0.1 seconds
    auto delay = cocos2d::DelayTime::create(0.1f);
    auto restoreColor = cocos2d::CallFunc::create([this]() {
        setColor(cocos2d::Color3B::WHITE); // Restore the original color
        });

    // Create a sequence of actions and assign a tag for identification
    auto sequence = cocos2d::Sequence::create(delay, restoreColor, nullptr);
    sequence->setTag(100);

    // Run the sequence
    runAction(sequence);
}

// Getter for Buitre's life points
int Buitre::getLife() {
    return _life;
}

// Initializes the Buitre object
bool Buitre::init() {
    // Create the sprite
    auto buitreSprite = Sprite::create();
    if (!buitreSprite->initWithFile("Buitre/Armature_animtion0_00.png")) {
        return false; // Return false if the sprite couldn't be initialized
    }
    setScale(0.7f); // Set the initial scale

    // Spawn Buitre at a random side of the screen
    spawnAtRandomSide();

    // Start the walk animation
    walkAnimation();

    // Schedule the update function
    scheduleUpdate();
    return true;
}

// Update function called every frame
void Buitre::update(float dt) {
    // Calculate direction towards the target (Bolillo)
    auto currentPosition = getPosition();
    auto direction = (_targetPosition - currentPosition).getNormalized();

    // Move Buitre towards the target
    auto newPosition = currentPosition + direction * _speed * dt;
    setPosition(newPosition);

    // Flip the sprite if necessary based on movement direction
    if ((direction.x > 0 && !_isFacingRight) || (direction.x < 0 && _isFacingRight)) {
        _isFacingRight = !_isFacingRight;
        setFlippedX(_isFacingRight); // Flip the sprite horizontally
    }
}

// Plays the walking animation
void Buitre::walkAnimation() {
    auto animation = cocos2d::Animation::create();

    // Add frames to the animation
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_00.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_01.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_02.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_03.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_04.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_05.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_06.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_07.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_08.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_09.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_10.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_11.png");
    animation->addSpriteFrameWithFile("Buitre/Armature_animtion0_12.png");
    animation->setDelayPerUnit(0.04f); // Set the delay between frames

    // Run the animation in a loop
    runAction(cocos2d::RepeatForever::create(cocos2d::Animate::create(animation)));
}

// Sets the target position (e.g., Bolillo's position)
void Buitre::setTargetPosition(const cocos2d::Vec2& targetPosition) {
    _targetPosition = targetPosition;
}

// Spawns Buitre at a random side of the screen
void Buitre::spawnAtRandomSide() {
    auto winSize = cocos2d::Director::getInstance()->getWinSize();

    // Generate a random side: 0 = left, 1 = right
    int side = cocos2d::random(0, 1);

    // Generate a random vertical position within the screen bounds
    float randomY = cocos2d::random(50.f, winSize.height - 50.f);

    // Set position based on the random side
    if (side == 0) {
        setPosition(cocos2d::Vec2(-50.f, randomY)); // Off the left edge
    }
    else {
        setPosition(cocos2d::Vec2(winSize.width + 50.f, randomY)); // Off the right edge
    }

    // Adjust the facing direction based on the side
    _isFacingRight = (side == 0); // If on the left, face right
    setFlippedX(_isFacingRight);
}

// Transforms Buitre into a boss with enhanced attributes
void Buitre::BossMode() {
    // Increase size
    setScale(1.3f);

    // Use pre-multiplied alpha blending for effects
    setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);

    // Set life to 100
    _life = 55;

    // Create the health bar if it doesn't already exist
    if (!_healthBar) {
        auto barBackground = Sprite::create("health_empty.png"); // Health bar background
        barBackground->setPosition(cocos2d::Vec2(200, 350));
        barBackground->setScale(0.8f);
        addChild(barBackground);

        auto barForeground = Sprite::create("health_full.png"); // Health bar foreground
        _healthBar = cocos2d::ProgressTimer::create(barForeground);
        _healthBar->setType(cocos2d::ProgressTimer::Type::BAR);
        _healthBar->setMidpoint(cocos2d::Vec2(0, 0.5f)); // Start from the left
        _healthBar->setBarChangeRate(cocos2d::Vec2(1, 0)); // Fill horizontally
        _healthBar->setPercentage(100.0f); // Initially full
        _healthBar->setPosition(cocos2d::Vec2(200, 350));
        _healthBar->setScale(0.8f);
        addChild(_healthBar);
    }
}
