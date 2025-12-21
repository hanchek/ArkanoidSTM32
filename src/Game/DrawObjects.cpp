#include "DrawObjects.h"

DrawObject::DrawObject(uint8_t x, uint8_t y)
{
    _x = x;
    _y = y;
    _prevX = x;
    _prevY = y;
}

void DrawObject::Draw(Display& display)
{
    if (_x != _prevX || _y != _prevY || IsDirty())
    {
        OnDraw(display, _prevX, _prevY, false);
        OnDraw(display, _x, _y, true);
        _prevX = _x;
        _prevY = _y;
        _dirty = false;
    }
}

DrawRectObject::DrawRectObject(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
    : DrawObject(x, y)
{
    _width = width;
    _height = height;
}

void DrawRectObject::OnDraw(Display& display, uint8_t x, uint8_t y, bool color) const
{
    display.DrawRect(x, y, _width, _height, color);
}

DrawCircleObject::DrawCircleObject(uint8_t x, uint8_t y, uint8_t radius, bool fill)
    : DrawObject(x, y)
{
    _radius = radius;
    _fill = fill;
}

void DrawCircleObject::OnDraw(Display& display, uint8_t x, uint8_t y, bool color) const
{
    display.DrawCircle(x, y, _radius, color, _fill);
}