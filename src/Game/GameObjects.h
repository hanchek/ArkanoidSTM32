#pragma once

#include "DrawObjects.h"

class Ball : public DrawCircleObject
{
public:
    Ball();

    void SetXF(float xf) { _xf = xf; }
    void SetYF(float yf) { _yf = yf; }
    float GetXF() const { return _xf; }
    float GetYF() const { return _yf; }

    void SetVelocityX(float dx) { _dx = dx; }
    void SetVelocityY(float dy) { _dy = dy; }
    float GetVelocityX() const { return _dx; }
    float GetVelocityY() const { return _dy; }

    Circle GetCircle() const override { return {_xf, _yf, _radius + 1.f}; }

    void Update(float dt);
private:
    float _dx;
    float _dy;
    float _xf;
    float _yf;
};

class Platform : public DrawRectObject
{
public:
    Platform();

    void Update(float dt);

    void SetLeftPressed(bool leftPressed) { _leftPressed = leftPressed; }
    void SetRightPressed(bool rightPressed) { _rightPressed = rightPressed; }

private:
    float _xf;
    bool _leftPressed = false;
    bool _rightPressed = false;
};

class Brick : public DrawRectObject
{
public:
    Brick(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

    Rect GetRect() const override { return {_x - 0.5f, _y - 0.5f, _width + 1.f, _height + 1.f}; }

    bool operator==(const Brick& other) const
    {
        return _x == other._x && _y == other._y;
    }
};