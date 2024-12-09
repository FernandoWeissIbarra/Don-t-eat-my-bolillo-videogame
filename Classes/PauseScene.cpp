#include "PauseScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

PauseScene* PauseScene::create()
{
    PauseScene* ret = new (std::nothrow) PauseScene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool PauseScene::init()
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
    expLabel = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf", 90);
    expLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 + 50
    ));
    this->addChild(expLabel);

    // Create the "Back to Main Menu" button
    auto backButton = ui::Button::create(
        "MainMenuButtonN.png", // Image for the normal state
        "MainMenuButtonP.png"  // Image for the pressed state
    );

    // Set button position
    backButton->setPosition(Vec2((visibleSize.width / 2) + 150, visibleSize.height / 2 - 100));

    // Set callback for button click
    backButton->addClickEventListener([=](Ref* sender) {
        this->goToMainMenu(sender); // Call the function when button is clicked
    });

    // Add the button to the scene
    this->addChild(backButton);

    // Create the "Back to Main Menu" button
    auto continueButton = ui::Button::create(
        "ContinueButtonN.png", // Image for the normal state
        "ContinueButtonP.png"  // Image for the pressed state
    );

    // Set button position
    continueButton->setPosition(Vec2((visibleSize.width / 2) + 150, visibleSize.height / 2 - 200));

    // Set callback for button click
    continueButton->addClickEventListener([=](Ref* sender) {
        Director::getInstance()->popScene(); // Call the function when button is clicked
    });

    // Add the button to the scene
    this->addChild(continueButton);


    return true;
}

// Callback function to transition to MainMenuScene
void PauseScene::goToMainMenu(Ref* sender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainMenuScene::create(), Color3B::BLACK)); // Replace the current scene with MainMenuScene
}