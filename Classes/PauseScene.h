#pragma once
#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "GameLoopScene.h"
#include "MainMenuScene.h"

class PauseScene : public cocos2d::Scene {
public:
    static PauseScene* create();
    virtual bool init();

    void goToMainMenu(Ref* sender);

private:
    cocos2d::Label* expLabel;
};

#endif // __PAUSE_SCENE_H__