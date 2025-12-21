#pragma once

#include "ssd1306/Display.h"
#include "Utils.h"

class DrawObject
{
    public:
        DrawObject(uint8_t x, uint8_t y);
        void Draw(Display& display);
        virtual void OnDraw(Display& display, uint8_t x, uint8_t y, bool color) const = 0;

        uint8_t GetX() const { return _x; }
        uint8_t GetY() const { return _y; }

        bool IsDirty() const { return _dirty; }
        void SetDirty(bool dirty) { _dirty = dirty; }

    protected:
        uint8_t _x;
        uint8_t _y;
        uint8_t _prevX;
        uint8_t _prevY;
        bool _dirty = true;
};

class DrawRectObject : public DrawObject
{
    public:
        DrawRectObject(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

        void OnDraw(Display& display, uint8_t x, uint8_t y, bool color) const override;

        uint8_t GetWidth() const { return _width; }
        uint8_t GetHeight() const { return _height; }

        Rect GetRect() const { return {_x, _y, _width, _height}; }

    protected:
        uint8_t _width;
        uint8_t _height;
};

class DrawCircleObject : public DrawObject
{
    public:
        DrawCircleObject(uint8_t x, uint8_t y, uint8_t radius, bool fill = false);

        void OnDraw(Display& display, uint8_t x, uint8_t y, bool color) const override;

        uint8_t GetRadius() const { return _radius; }

        virtual Circle GetCircle() const { return {_x, _y, _radius}; }

    protected:
        uint8_t _radius;
        bool _fill;
};  
