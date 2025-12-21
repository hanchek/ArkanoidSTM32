struct Rect
{
    float x;
    float y;
    float w;
    float h;
};

struct Circle
{
    float x;
    float y;
    float r;
};

struct Line
{
    float x1;
    float y1;
    float x2;
    float y2;
};

enum class CollisionSide
{
    None,
    Top,
    Bottom,
    Left,
    Right
};

bool Intersects(const Circle& circle, const Rect& rect);
CollisionSide GetCollisionSide(const Circle& circle, const Rect& rect);

bool IntersectsVerticalWall(const Circle& circle, float x);
bool IntersectsHorizontalWall(const Circle& circle, float y);

