#pragma once
#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(MainMenu);

    cocos2d::Sprite* background;
    cocos2d::Sprite* track1;
    cocos2d::Sprite* track2;
};