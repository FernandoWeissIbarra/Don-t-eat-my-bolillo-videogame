#pragma once
#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "GameLoopScene.h"

class GameOverScene : public cocos2d::Scene {
public:
    static GameOverScene* create(int exp, bool isWin);
    virtual bool init(int exp, bool isWin);

    void goToMainMenu(Ref* sender);

private:
    cocos2d::Label* expLabel;
    cocos2d::Label* resultLabel;
};

#endif // __GAME_OVER_SCENE_H__