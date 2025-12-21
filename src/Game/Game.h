#pragma once

#include <vector>
#include <memory>

#include "ssd1306/Display.h"
#include "GameObjects.h"

class Game
{
public:
    void Init();
    void Update(float dt);
    void Draw(Display& display);
    void OnLeftPressed();
    void OnRightPressed();

private:
    void UpdateCollisions();

    std::vector<Brick> _bricks;
    std::vector<Rect> _rectsToClear;
    Ball _ball;
    Platform _platform;
    float _pressTimeOut  = 0.f;
    float _gameOverTimeOut = 0.f;
    bool _needClearDisplay = false;
};

bool CircleIntersectsRect(float cx, float cy, float radius,
                          float rx, float ry, float rw, float rh);