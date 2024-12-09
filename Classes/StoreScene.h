#pragma once
// StoreScene.h
#ifndef __STORE_SCENE_H__
#define __STORE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameLoopScene;

class StoreScene : public cocos2d::Scene {
public:
    // Create scene with current EXP and character stats
    static StoreScene* create(long currentEXP, int currentLife, float currentSpeed, GameLoopScene* gameLoopScene);

    // Initialize the scene with passed parameters
    virtual bool init(long currentEXP, int currentLife, float currentSpeed, GameLoopScene* gameLoopScene);

    static Scene* createScene(long currentEXP, int currentLife, float currentSpeed);

private:
    // UI Labels
    cocos2d::Label* _expLabel;
    cocos2d::Label* _lifeLabel;
    cocos2d::Label* _speedLabel;
    
    // Upgrade buttons
    cocos2d::ui::Button* _lifeUpgradeBtn;
    cocos2d::ui::Button* _speedUpgradeBtn;
    
    // Current game stats
    long _currentEXP;
    int _currentLife;
    float _currentSpeed;
    
    // Upgrade costs
    const int LIFE_UPGRADE_COST = 100;
    const int SPEED_UPGRADE_COST = 100;
    
    // Upgrade methods
    void upgradeLife();
    void upgradeSpeed();
    
    // Update UI method
    void updateUI();

    // Continue to next scene
    void continueToGameLoop();

    GameLoopScene* _gameLoopScene;
};

#endif // __STORE_SCENE_H__