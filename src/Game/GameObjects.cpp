#include "GameObjects.h"

#include <algorithm>

constexpr uint8_t FIELD_WIDTH = 128;
constexpr uint8_t FIELD_HEIGHT = 64;
constexpr float PLATFORM_SPEED = 160.f;
constexpr uint8_t BALL_RADIUS = 2;
constexpr bool BALL_FILL = true;
constexpr uint8_t BRICK_WIDTH = 7;
constexpr uint8_t BRICK_HEIGHT = 3;

Ball::Ball()
    : DrawCircleObject(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, BALL_RADIUS, BALL_FILL)
{
    _xf = static_cast<float>(_x);
    _yf = static_cast<float>(_y);
}

void Ball::Update(float dt)
{
    _xf += _dx * dt;
    _yf += _dy * dt;

    _x = static_cast<uint8_t>(_xf);
    _y = static_cast<uint8_t>(_yf);
}

Platform::Platform() : DrawRectObject(FIELD_WIDTH / 2, 0, 20, 3)
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
    _x = static_cast<uint8_t>(_xf);
}

Brick::Brick(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
    : DrawRectObject(x, y, w, h)
{
}
