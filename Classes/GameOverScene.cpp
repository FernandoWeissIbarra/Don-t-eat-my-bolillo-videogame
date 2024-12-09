#include "GameOverScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

GameOverScene* GameOverScene::create(int exp, bool isWin)
{
    GameOverScene* ret = new (std::nothrow) GameOverScene();
    if (ret && ret->init(exp, isWin))
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool GameOverScene::init(int exp, bool isWin)
{
    if (!Scene::init())
    {
        return false;
    }

    // Get screen size
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Background
    auto background = cocos2d::Sprite::create("Background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, -1);

    // Create result label (Win/Lose)
    resultLabel = Label::createWithTTF(
        isWin ? "Congratulations! You Won!" : "Game Over!",
        "fonts/Marker Felt.ttf", 90
    );
    resultLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 + 50
    ));
    this->addChild(resultLabel);

    // Create EXP label
    expLabel = Label::createWithTTF(
        "EXP Earned: " + std::to_string(exp),
        "fonts/Marker Felt.ttf", 60
    );
    expLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 20
    ));
    this->addChild(expLabel);

    // Create the "Back to Main Menu" button
        auto backButton = MenuItemImage::create(
            "MainMenuButtonN.png", // Image for the normal state
            "MainMenuButtonP.png", // Image for the pressed state
            CC_CALLBACK_1(GameOverScene::goToMainMenu, this) // Callback function for the button click
        );

    // Create menu with the button
    auto menu = Menu::create(backButton, nullptr);
    menu->setPosition(Vec2((visibleSize.width / 2) + 150, visibleSize.height / 2 - 100)); // Position of the button
    this->addChild(menu);


    return true;
}

// Callback function to transition to MainMenuScene
void GameOverScene::goToMainMenu(Ref* sender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainMenuScene::create(), Color3B::BLACK)); // Replace the current scene with MainMenuScene
}