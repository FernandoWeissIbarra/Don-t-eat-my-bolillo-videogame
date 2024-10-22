// MainMenuScene.cpp

#include "MainMenuScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene() {
    return MainMenuScene::create();
}

bool MainMenuScene::init() {
    // Call parent class init
    if (!Scene::init()) {
        return false;
    }

    // Get window size to position elements
    auto winSize = Director::getInstance()->getWinSize();

    // Create a label for the title
    auto label = Label::createWithTTF("Bolillo - Main Menu", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(winSize.width / 2, winSize.height / 1.5));
    this->addChild(label);

    // Just displaying a label for now, no buttons or transitions

    return true;
}

