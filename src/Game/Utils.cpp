#include "Utils.h"

#include <algorithm>

CollisionSide GetCollisionSide(const Circle& circle, const Rect& rect)
{
    float nearestX = std::max(rect.x, std::min(circle.x, rect.x + rect.w));
    float nearestY = std::max(rect.y, std::min(circle.y, rect.y + rect.h));

    float deltaX = circle.x - nearestX;
    float deltaY = circle.y - nearestY;
    float distanceSquared = deltaX * deltaX + deltaY * deltaY;

    if (distanceSquared >= (circle.r * circle.r))
    {
        return CollisionSide::None;
    }

    // Determine if nearest point is on an edge or corner
    float leftEdge = rect.x;
    float rightEdge = rect.x + rect.w;
    float bottomEdge = rect.y;
    float topEdge = rect.y + rect.h;

    const bool isOnLeftEdge = (nearestX == leftEdge);
    const bool isOnRightEdge = (nearestX == rightEdge);
    const bool isOnBottomEdge = (nearestY == bottomEdge);
    const bool isOnTopEdge = (nearestY == topEdge);

    const bool isCornerCollision = (isOnLeftEdge || isOnRightEdge) && (isOnBottomEdge || isOnTopEdge);

    if (isCornerCollision)
    {
        float penetrationLeft = (circle.x + circle.r) - leftEdge;
        float penetrationRight = rightEdge - (circle.x - circle.r);
        float penetrationBottom = (circle.y + circle.r) - bottomEdge;
        float penetrationTop = topEdge - (circle.y - circle.r);

        float minPenetration = penetrationLeft;
        CollisionSide side = CollisionSide::Left;

        if (isOnRightEdge && penetrationRight < minPenetration)
        {
            minPenetration = penetrationRight;
            side = CollisionSide::Right;
        }
        if (isOnBottomEdge && penetrationBottom < minPenetration)
        {
            minPenetration = penetrationBottom;
            side = CollisionSide::Bottom;
        }
        if (isOnTopEdge && penetrationTop < minPenetration)
        {
            minPenetration = penetrationTop;
            side = CollisionSide::Top;
        }

        return side;
    }

    if (isOnLeftEdge)
        return CollisionSide::Left;
    if (isOnRightEdge)
        return CollisionSide::Right;
    if (isOnBottomEdge)
        return CollisionSide::Bottom;
    if (isOnTopEdge)
        return CollisionSide::Top;

    return CollisionSide::None;
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
