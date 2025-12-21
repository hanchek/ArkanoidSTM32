#include "Utils.h"

#include <algorithm>

CollisionSide GetCollisionSide(const Circle& circle, const Rect& rect)
{
    // First check if there's an intersection
    float nearestX = std::max(rect.x, std::min(circle.x, rect.x + rect.w));
    float nearestY = std::max(rect.y, std::min(circle.y, rect.y + rect.h));

    float deltaX = circle.x - nearestX;
    float deltaY = circle.y - nearestY;

    if ((deltaX * deltaX + deltaY * deltaY) >= (circle.r * circle.r))
    {
        return CollisionSide::None;
    }

    // Calculate distances to each side
    float distToLeft = std::abs(circle.x - rect.x);
    float distToRight = std::abs(circle.x - (rect.x + rect.w));
    float distToBottom = std::abs(circle.y - rect.y);
    float distToTop = std::abs(circle.y - (rect.y + rect.h));

    // Find the side with minimum distance
    float minDist = distToLeft;
    CollisionSide side = CollisionSide::Left;

    if (distToRight < minDist)
    {
        minDist = distToRight;
        side = CollisionSide::Right;
    }
    if (distToTop < minDist)
    {
        minDist = distToTop;
        side = CollisionSide::Top;
    }
    if (distToBottom < minDist)
    {
        side = CollisionSide::Bottom;
    }

    return side;
}

bool Intersects(const Circle& circle, const Rect& rect)
{
    return GetCollisionSide(circle, rect) != CollisionSide::None;
}

bool IntersectsVerticalWall(const Circle& circle, float x)
{
    return std::abs(circle.x - x) < circle.r;
}

bool IntersectsHorizontalWall(const Circle& circle, float y)
{
    return std::abs(circle.y - y) < circle.r;
}
