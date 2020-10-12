#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "Utils.h"
#include "Game.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

bool MainMenu::init()
{
    if (!Scene::init() )
        return false;

    this->scheduleUpdate();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    background = Sprite::create("Background.png");
    auto title = Sprite::create("Title.png");
    title->setAnchorPoint(Vec2(0, 0));
    title->setPosition(33.0f, 609.0f);
    background->addChild(title, 0);

    track1 = Sprite::create("Background.png", Rect(0, 707, 1024, 11));
    track1->setAnchorPoint(Vec2(0, 0));
    track1->setPosition(0, 51);

    track2 = Sprite::create("Background.png", Rect(0, 707, 1024, 11));
    track2->setAnchorPoint(Vec2(0, 0));
    track2->setPosition(1024, 51);

    background->addChild(track1);
    background->addChild(track2);

    background->setAnchorPoint(Vec2(0, 0));
    background->setPosition(Vec2(0, 0));
    this->addChild(background, 0);

    auto button = ui::Button::create("StartNormal.png", "StartSelected.png");
    button->setPosition(Vec2(512.0f, 150.0f));
    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if(type == ui::Widget::TouchEventType::BEGAN)
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, Game::createScene(), Color3B(0, 0, 0)));
    });

    background->addChild(button);

    auto anim = Animation::create();
    anim->addSpriteFrameWithFile("f1.png");
    anim->addSpriteFrameWithFile("f2.png");
    anim->addSpriteFrameWithFile("f3.png");
    anim->addSpriteFrameWithFile("f4.png");

    anim->setLoops(-1);
    anim->setDelayPerUnit(0.1f);
  
    Animate* theAnim = Animate::create(anim);
    Sprite* bear = Sprite::create("f1.png");
    bear->setPosition(256, 400);

    addChild(bear, 0);
    bear->runAction(theAnim);

    return true;
}

void MainMenu::update(float dt) {
    const float offset = 150.0f;
    float length = 2048.0f;
    track1->setPositionX(wrap(track1->getPositionX() - offset * dt, length));
    track2->setPositionX(wrap(track2->getPositionX() - offset * dt, length));
}