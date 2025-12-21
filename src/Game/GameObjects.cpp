#include "GameObjects.h"

#include <algorithm>
#include <cmath>

constexpr uint8_t FIELD_WIDTH = 128;
constexpr uint8_t FIELD_HEIGHT = 64;
constexpr float PLATFORM_SPEED = 160.f;
constexpr uint8_t BALL_RADIUS = 2;
constexpr bool BALL_FILL = true;
constexpr uint8_t BRICK_WIDTH = 7;
constexpr uint8_t BRICK_HEIGHT = 3;
constexpr uint8_t PLATFORM_WIDTH = 20;
constexpr uint8_t PLATFORM_HEIGHT = 3;
constexpr uint8_t BALL_START_X = DISPLAY_WIDTH / 2 - 1;
constexpr uint8_t BALL_START_Y = PLATFORM_HEIGHT + 5;

Ball::Ball()
    : DrawCircleObject(BALL_START_X, BALL_START_Y, BALL_RADIUS, BALL_FILL)
{
    _xf = static_cast<float>(_x);
    _yf = static_cast<float>(_y);
}

void Ball::Update(float dt)
{
    _xf += _dx * dt;
    _yf += _dy * dt;

    _x = std::round(_xf);
    _y = std::round(_yf);
}

Platform::Platform() : DrawRectObject((FIELD_WIDTH - PLATFORM_WIDTH) / 2, 0, PLATFORM_WIDTH, PLATFORM_HEIGHT)
{
    _xf = static_cast<float>(_x);
}

void Platform::Update(float dt)
{
    if (_leftPressed)
    {
        _xf -= PLATFORM_SPEED * dt;
        _leftPressed = false;
    }
    if (_rightPressed)
    {
        _xf += PLATFORM_SPEED * dt;
        _rightPressed = false;
    }

    _xf = std::clamp(_xf, 0.f, static_cast<float>(FIELD_WIDTH - _width));
    _x = std::round(_xf);
}

Brick::Brick(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
    : DrawRectObject(x, y, w, h)
{
}
