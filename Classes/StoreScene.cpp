// StoreScene.cpp
#include "StoreScene.h"
#include "GameLoopScene.h"
#include "cocos2d.h"

USING_NS_CC;

StoreScene* StoreScene::create(long currentEXP, int currentLife, float currentSpeed, GameLoopScene* gameLoopScene) {
    StoreScene* ret = new (std::nothrow) StoreScene();
    if (ret && ret->init(currentEXP, currentLife, currentSpeed, gameLoopScene)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool StoreScene::init(long currentEXP, int currentLife, float currentSpeed, GameLoopScene* gameLoopScene) {
    if (!Scene::init()) {
        return false;
    }

    // Store current stats
    _currentEXP = currentEXP;
    _currentLife = currentLife;
    _currentSpeed = currentSpeed;
    
    // Store a pointer to the game loop scene
    _gameLoopScene = gameLoopScene;

    // Get visible size and origin
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Background
    auto background = cocos2d::Sprite::create("Background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, -1);

    // Title
    auto titleLabel = Label::createWithTTF("CHARACTER UPGRADES", "fonts/Marker Felt.ttf", 90);
    titleLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - 100));
    this->addChild(titleLabel);

    // EXP Label
    _expLabel = Label::createWithTTF("Available EXP: " + std::to_string(_currentEXP), "fonts/Marker Felt.ttf", 40);
    _expLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - 200));
    this->addChild(_expLabel);

    // Current Stats Label
    _lifeLabel = Label::createWithTTF("Life: " + std::to_string(_currentLife), "fonts/Marker Felt.ttf", 30);
    _speedLabel = Label::createWithTTF("Speed: " + std::to_string(_currentSpeed), "fonts/Marker Felt.ttf", 30);
    
    _lifeLabel->setPosition(Vec2(visibleSize.width / 2 + 400, visibleSize.height - 300));
    _speedLabel->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height - 300));
    
    this->addChild(_lifeLabel);
    this->addChild(_speedLabel);
    

    // Upgrade Buttons
    _lifeUpgradeBtn = ui::Button::create("UpgradeButtonP.png", "UpgradeButtonPR.png");
    _speedUpgradeBtn = ui::Button::create("UpgradeButtonP.png", "UpgradeButtonPR.png");
    
    // Life Upgrade Button
    _lifeUpgradeBtn->setPosition(Vec2(visibleSize.width / 2 + 400, visibleSize.height - 400));
    _lifeUpgradeBtn->addClickEventListener(CC_CALLBACK_0(StoreScene::upgradeLife, this));
    this->addChild(_lifeUpgradeBtn);

    // Speed Upgrade Button
    _speedUpgradeBtn->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height - 400));
    _speedUpgradeBtn->addClickEventListener(CC_CALLBACK_0(StoreScene::upgradeSpeed, this));
    this->addChild(_speedUpgradeBtn);

    // Continue Button
    auto continueBtn = ui::Button::create("ContinueButtonN.png", "ContinueButtonP.png");
    continueBtn->setPosition(Vec2(visibleSize.width / 2 + 150, 100));
    continueBtn->addClickEventListener(CC_CALLBACK_0(StoreScene::continueToGameLoop, this));
    this->addChild(continueBtn);

    // Update button states based on available EXP
    updateUI();

    return true;
}

void StoreScene::upgradeLife() {
    if (_currentEXP >= LIFE_UPGRADE_COST) {
        _currentLife += 10;
        _currentEXP -= LIFE_UPGRADE_COST;
        updateUI();
    }
}

void StoreScene::upgradeSpeed() {
    if (_currentEXP >= SPEED_UPGRADE_COST) {
        _currentSpeed += 10.0f;
        _currentEXP -= SPEED_UPGRADE_COST;
        updateUI();
    }
}

void StoreScene::updateUI() {
    // Update EXP Label
    _expLabel->setString("Available EXP: " + std::to_string(_currentEXP));

    // Update Stats Labels
    _lifeLabel->setString("Life: " + std::to_string(_currentLife));
    _speedLabel->setString("Speed: " + std::to_string(_currentSpeed));
    
    // Update button states
    _lifeUpgradeBtn->setEnabled(_currentEXP >= LIFE_UPGRADE_COST);
    _speedUpgradeBtn->setEnabled(_currentEXP >= SPEED_UPGRADE_COST);
    }

void StoreScene::continueToGameLoop() {
    // Directly update player stats in the game loop scene
    if (_gameLoopScene) {
        _gameLoopScene->updatePlayerStats(_currentLife, _currentSpeed, _currentEXP);
    }

    // Return to the previous scene
    Director::getInstance()->popScene();
}

Scene* StoreScene::createScene(long currentEXP, int currentLife, float currentSpeed) {
    return StoreScene::create(currentEXP, currentLife, currentSpeed, nullptr);
}