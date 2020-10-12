#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);

    CREATE_FUNC(Game);

    void Game::onMouseClick(cocos2d::EventMouse* event);
private:
    void adjustPipePair(size_t index);
    void detectCollision();
    void detectPass();
    void updateScore();
    void gameOver();
    void restart();
    void readBestScore();
    void writeBestScore();

    cocos2d::Sprite* background;
    cocos2d::Sprite* track1;
    cocos2d::Sprite* track2;
    cocos2d::Sprite* pastPipe;
    cocos2d::Sprite* gameOverTitle;
    cocos2d::Sprite* scoreboard;
    cocos2d::ui::Button* menuButton;
    cocos2d::ui::Button* restartButton;

    std::vector<cocos2d::Sprite*> upperPipes;
    std::vector<cocos2d::Sprite*> lowerPipes;

    const size_t numPipePairsPerScreen = 3;
    const size_t totalPipePairCount = numPipePairsPerScreen + 1;
    const float gap = 1024.0f / numPipePairsPerScreen;
    const float g = 200.0f;

    cocos2d::Animation* anim;
    cocos2d::Animate* theAnim;
    cocos2d::Sprite* bird;
    float speed;
    cocos2d::Label* count;
    unsigned int score;
    unsigned int bestScore;
    bool run;

    cocos2d::Label* yourScoreLabel;
    cocos2d::Label* bestScoreLabel;
};