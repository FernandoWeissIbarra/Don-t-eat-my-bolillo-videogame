#pragma once

#ifndef __GAME_LOOP_SCENE_H__
#define __GAME_LOOP_SCENE_H__

#include "cocos2d.h"
#include "Bolillo.h"
#include "Pato.h"
#include "Bullet.h"
#include "Gabiota.h"
#include "Cuervo.h"
#include "Paloma.h"
#include "Buitre.h"
#include "GameOverScene.h"
#include "StoreScene.h"
#include "PauseScene.h"

class GameLoopScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void update(float dt) override;

    void patoColision();

    void addPatos(int number);

    void checkPatoBulletCollisions();

    void gabiotaColision();

    void addGabiotas(int number);

    void checkGabiotaBulletCollisions();
    
    void cuervoColision();

    void addCuervos(int number);

    void checkCuervoBulletCollisions();

    void palomaColision();

    void addPalomas(int number);

    void checkPalomaBulletCollisions();

    void buitreColision();

    void addBuitres(int number);

    void checkBuitreBulletCollisions();

    cocos2d::Vector<Bullet*> _bullets;

    void goToGameOverScene(bool des);

    void goToStoreScene();

    void updatePlayerStats(int life, float speed, long _EXP);

    void setEXP(long EXP);

    CREATE_FUNC(GameLoopScene);

private:
    Bolillo* _bolillo = nullptr;
    bool isBolilloBeingDamaged = false;
    // Contenedor para almacenar los patos

    Bullet* _bullet = nullptr;

    cocos2d::Label* _expLabel;

    long EXP;

    cocos2d::Vector<Pato*> _patos; 
    bool patoFlag = false;
    bool patoBoss = false;

    cocos2d::Vector<Gabiota*> _gabiotas;
    bool gabiotaFlag = false;
    bool gabiotaBoss = false;

    cocos2d::Vector<Cuervo*> _cuervos;
    bool cuervoFlag = false;
    bool cuervoBoss = false;

    cocos2d::Vector<Paloma*> _palomas;
    bool palomaFlag = false;
    bool palomaBoss = false;

    cocos2d::Vector<Buitre*> _buitres;
    bool buitreFlag = false;
    bool buitreBoss = false; 
};

#endif // __MAIN_MENU_SCENE_H__