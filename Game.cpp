#include "Game.h"
#include "Utils.h"
#include <cmath>
#include "MainMenu.h"

USING_NS_CC;

Scene* Game::createScene()
{
    return Game::create();
}

bool Game::init()
{
    if (!Scene::init())
        return false;

    bestScore = 0;
    readBestScore();

    this->scheduleUpdate();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    count = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 76);
    count->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - count->getContentSize().height));
    this->addChild(count, 1);

    background = Sprite::create("Background.png");
    track1 = Sprite::create("Background.png", Rect(0, 707, 1024, 11));
    track1->setAnchorPoint(Vec2(0, 0));
    track1->setPosition(0, 51);
    background->addChild(track1);

    track2 = Sprite::create("Background.png", Rect(0, 707, 1024, 11));
    track2->setAnchorPoint(Vec2(0, 0));
    track2->setPosition(1024, 51);
    background->addChild(track2);

    
    auto offset = 1024.0f + gap;
    for (auto i = 0; i < totalPipePairCount; ++i)
    {
        auto upperPipe = Sprite::create("Pipe.png", Rect(0, 0, 67, 352));
        upperPipe->setFlippedY(true);
        upperPipe->setAnchorPoint(Vec2(1, 1));
        upperPipe->setPosition(offset, 768);
        background->addChild(upperPipe);
        upperPipes.push_back(upperPipe);

        auto lowerPipe = Sprite::create("Pipe.png", Rect(0, 0, 67, 352));
        lowerPipe->setAnchorPoint(Vec2(1, 0));
        lowerPipe->setPosition(offset, 65);
        background->addChild(lowerPipe);
        lowerPipes.push_back(lowerPipe);
        offset += gap;
        adjustPipePair(i);
    }

    pastPipe = nullptr;

    background->setAnchorPoint(Vec2(0, 0));
    background->setPosition(Vec2(0, 0));
    this->addChild(background, 0);

    bird = Sprite::create("f1.png");
    bird->setPosition(256, 400);
    background->addChild(bird, 0);

    gameOverTitle = Sprite::create("GameOver.png");
    gameOverTitle->setVisible(false);
    gameOverTitle->setPosition(512.0f, 700.0f);
    background->addChild(gameOverTitle);

    scoreboard = Sprite::create("Scoreboard.png");
    scoreboard->setVisible(false);
    scoreboard->setPosition(512.0f, 500.0f);
    background->addChild(scoreboard);

    yourScoreLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 40);
    yourScoreLabel->setTextColor(Color4B::BLACK);
    yourScoreLabel->setPosition(450, 145);
    scoreboard->addChild(yourScoreLabel);

    bestScoreLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 40);
    bestScoreLabel->setTextColor(Color4B::BLACK);
    bestScoreLabel->setPosition(450,90);
    scoreboard->addChild(bestScoreLabel);

    menuButton = ui::Button::create("MenuNormal.png", "MenuSelected.png");
    menuButton->setVisible(false);
    menuButton->setPosition(Vec2(300.0f, 200.0f));
    menuButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN)
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainMenu::createScene(), Color3B(0, 0, 0)));
    });
    background->addChild(menuButton);

    restartButton = ui::Button::create("RestartNormal.png", "RestartSelected.png");
    restartButton->setVisible(false);
    restartButton->setPosition(Vec2(700.0f, 200.0f));
    restartButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN)
            restart();
    });
    background->addChild(restartButton);

    anim = Animation::create();
    anim->addSpriteFrameWithFile("f1.png");
    anim->addSpriteFrameWithFile("f2.png");
    anim->addSpriteFrameWithFile("f3.png");
    anim->addSpriteFrameWithFile("f4.png");
    anim->setDelayPerUnit(0.1f);

    theAnim = Animate::create(anim);
    CC_SAFE_RETAIN(anim);
    
    auto listener0 = EventListenerMouse::create();
    listener0->onMouseDown = CC_CALLBACK_1(Game::onMouseClick, this);
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener0, this);
    run = true;

    return true;
}

void Game::update(float dt) {
    if (run) {
        detectCollision();
        detectPass();

        speed -= g * dt;
        bird->setPositionY(bird->getPositionY() + speed * dt);

        const float offset = 150.0f;
        float length = 2048.0f;

        track1->setPositionX(wrap(track1->getPositionX() - offset * dt, length));
        track2->setPositionX(wrap(track2->getPositionX() - offset * dt, length));

        for (auto i = 0; i < totalPipePairCount; ++i)
        {
            upperPipes[i]->setPositionX(upperPipes[i]->getPositionX() - offset * dt);
            lowerPipes[i]->setPositionX(lowerPipes[i]->getPositionX() - offset * dt);

            if (upperPipes[i]->getPositionX() < 0)
                adjustPipePair(i);
        }
    }
}

void Game::onMouseClick(EventMouse* event)
{
    auto up = cocos2d::RotateTo::create(2, -45.0f);
    auto down = cocos2d::RotateTo::create(2, 90.0f);
    auto seq = cocos2d::Sequence::createWithTwoActions(up, down);
    bird->runAction(seq);
    bird->runAction(Animate::create(anim));
    speed += g;
}

void Game::adjustPipePair(size_t index)
{
    float x1 = upperPipes[index]->getPositionX();
    upperPipes[index]->setPositionX((1024 + gap) - x1);
    lowerPipes[index]->setPositionX((1024 + gap) - x1);

    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<int> distribution(0, 426);

    float v1 = distribution(engine);
    float v2 = v1 + 71.0f;
    float v3 = v1 - 71.0f;
    float v4 = 100.0f + v3;
    float v5 = 426.0f - v2;
    float v6 = 100.0f + v5;

    upperPipes[index]->setTextureRect(Rect(0, 0, 67, v6));
    lowerPipes[index]->setTextureRect(Rect(0, 0, 67, v4));
}

void Game::detectCollision()
{
    auto position = bird->getPosition();
    auto angle = bird->getRotation() * (M_PI / 180.0f);
    auto birdSize = cocos2d::Vec2(bird->getTextureRect().size.width, bird->getTextureRect().size.height);

    auto lb = (bird->getPosition() - birdSize / 2.0f).rotateByAngle(position, -angle);
    auto lt = (bird->getPosition() + birdSize / 2.0f).rotateByAngle(position, -angle);
    auto rt = (bird->getPosition() + birdSize / 2.0f).rotateByAngle(position, -angle);
    auto rb = (bird->getPosition() + birdSize / 2.0f).rotateByAngle(position, -angle);

    for (size_t i = 0; i < totalPipePairCount; ++i)
    {
        const auto& upperPipeTextureRect = upperPipes[i]->getTextureRect();
        auto upperPipePosition = upperPipes[i]->getPosition() - cocos2d::Vec2(upperPipeTextureRect.size.width, upperPipeTextureRect.size.height);

        const auto& lowerPipeTextureRect = lowerPipes[i]->getTextureRect();
        auto lowerPipePosition = lowerPipes[i]->getPosition() - cocos2d::Vec2(upperPipeTextureRect.size.width, 0.0f);

        auto upperPipeBB = cocos2d::Rect(upperPipePosition, upperPipeTextureRect.size);
        auto lowerPipeBB = cocos2d::Rect(lowerPipePosition, lowerPipeTextureRect.size);

        if (upperPipeBB.containsPoint(lb) || upperPipeBB.containsPoint(lt) ||  upperPipeBB.containsPoint(rt) || upperPipeBB.containsPoint(rb) ||
            lowerPipeBB.containsPoint(lb) || lowerPipeBB.containsPoint(lt) || lowerPipeBB.containsPoint(rt) || lowerPipeBB.containsPoint(rb))
            gameOver();
    }

    const float groundLevel = 60.0f;
    if (lb.y <= groundLevel || rb.y <= groundLevel)
        gameOver();
}

void Game::detectPass()
{
    auto position = bird->getPosition();
    auto angle = bird->getRotation() * (M_PI / 180.0f);
    auto birdSize = cocos2d::Vec2(bird->getTextureRect().size.width, bird->getTextureRect().size.height);
    auto lb = (bird->getPosition() - birdSize / 2.0f).rotateByAngle(position, -angle);

    for (size_t i = 0; i < totalPipePairCount; ++i)
    {
        auto upperPipePosition = upperPipes[i]->getPosition();
        if (upperPipePosition.x > 0.0f && upperPipePosition.x < lb.x)
        {
            if (pastPipe == nullptr || pastPipe != upperPipes[i])
            {
                pastPipe = upperPipes[i];
                updateScore();
            }
        }
    }
}

void Game::updateScore()
{
    count->setString(std::to_string(++score));
}

void Game::gameOver()
{
    speed = 0.0f;
    run = false;

    bird->stopAllActions();
    auto fallDown = cocos2d::MoveBy::create(0.4f, Vec2(0.0, 100 - bird->getPositionY()));
    auto rotate = cocos2d::RotateTo::create(0.1f, 90.0f);

    auto showScoreboard = CallFunc::create([this] {
        if (score > bestScore)
        {
            bestScore = score;
            writeBestScore();
        }

        yourScoreLabel->setString(std::to_string(score));
        bestScoreLabel->setString(std::to_string(bestScore));
        count->setVisible(false);
        gameOverTitle->setVisible(true);
        scoreboard->setVisible(true);
        menuButton->setVisible(true);
        restartButton->setVisible(true);
    });

    auto seq = cocos2d::Sequence::create({ rotate, fallDown, showScoreboard });
    bird->runAction(seq);
}

void Game::restart()
{
    run = true;
    bird->setPosition(256, 400);
    bird->setRotation(0.0f);

    count->setVisible(true);
    count->setString(std::to_string(score = 0));

    gameOverTitle->setVisible(false);
    scoreboard->setVisible(false);
    menuButton->setVisible(false);
    restartButton->setVisible(false);

    auto offset = 1024.0f + gap;
    for (auto i = 0; i < totalPipePairCount; ++i)
    {
        auto upperPipe = upperPipes[i];
        upperPipe->setFlippedY(true);
        upperPipe->setAnchorPoint(Vec2(1, 1));
        upperPipe->setPosition(offset, 768);

        auto lowerPipe = lowerPipes[i];
        lowerPipe->setAnchorPoint(Vec2(1, 0));
        lowerPipe->setPosition(offset, 65);
        offset += gap;
        adjustPipePair(i);
    }

    pastPipe = nullptr;
}

void Game::readBestScore()
{
    FILE* file = fopen("bestScore.txt", "r");
    if (!file)
        writeBestScore();

    fscanf(file, "%u", &bestScore);
    fclose(file);
}

void Game::writeBestScore()
{
    FILE* file = fopen("bestScore.txt", "w");
    fprintf(file, "%u", bestScore);
    fclose(file);
}