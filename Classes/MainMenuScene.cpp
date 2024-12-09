// MainMenuScene.cpp

#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include "GameLoopScene.h"
#include "audio/include/AudioEngine.h"

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
    Size winSize = Director::getInstance()->getWinSize();
    
    Sprite* sprite = Sprite::create("TitleScreen.png");
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    // add the sprite as a child to this layer
    sprite->setScale(0.8f);
    this->addChild(sprite, -1);

    //Create title sprite
    Label* title = Label::createWithTTF("No te comas mi Bolillo", "fonts/Marker Felt.ttf", 90);
    title->setPosition(Vec2(winSize.width / 2, (winSize.height / 2) + 200));

    this->addChild(title);

    //Create the start button
    auto playButton = ui::Button::create("BotonStartN.png", "BotonStartP.png");
    playButton->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    playButton->addClickEventListener([](Ref* sender) {
        auto gameLoopScene = GameLoopScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameLoopScene));
    });

    this->addChild(playButton);

    //Create exit button
    auto exitButton = ui::Button::create("BotonExitN.png", "BotonExitP.png");
    exitButton->setPosition(Vec2(winSize.width / 2, (winSize.height / 2) - 100));
    exitButton->addClickEventListener([](Ref* sender) {
        AudioEngine::stopAll();
        // Stop the game and exit
        Director::getInstance()->end();
    });

    auto glview = Director::getInstance()->getOpenGLView();

    this->addChild(exitButton);

    return true;

}

// Implement CREATE_FUNC for MainMenuScene
CREATE_FUNC(MainMenuScene);