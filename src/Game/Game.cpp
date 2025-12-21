#include "Game.h"

#include "GameOver.h"

#include <algorithm>
#include <cmath>

constexpr float PRESS_TIMEOUT = 0.03f;
constexpr float GAME_OVER_TIMEOUT = 1.f;
constexpr uint8_t BRICK_WIDTH = 15;
constexpr uint8_t BRICK_HEIGHT = 7;
constexpr float BALL_SPEED = 90.f;
constexpr float BALL_MIN_ANGLE = 30.f * PI / 180.f;
constexpr float BALL_MAX_ANGLE = 150.f * PI / 180.f;


void Game::Init()
{
    constexpr int columns = DISPLAY_WIDTH / (BRICK_WIDTH + 1);
    constexpr int rows = 3;

    _bricks.clear();
    for (int i = 0; i < columns; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            const uint8_t w = BRICK_WIDTH + 1;
            const uint8_t h = BRICK_HEIGHT + 1;
            const uint8_t x = i * w;
            const uint8_t y = DISPLAY_HEIGHT - (j + 1) * h;
            _bricks.push_back(Brick(x, y, BRICK_WIDTH, BRICK_HEIGHT, 3 - j));
        }
    }

    _ball = Ball();
    _ball.SetVelocityX(0.f);
    _ball.SetVelocityY(90.f);
    _platform = Platform();
}

void Game::Update(float dt)
{
    if (_pressTimeOut > 0.f)
    {
        _pressTimeOut -= dt;
    }

    if (_gameOverTimeOut > 0.f)
    {
        _gameOverTimeOut -= dt;
        if (_gameOverTimeOut <= 0.f)
        {
            _gameOverTimeOut = 0.f;
            _needClearDisplay = true;
            Init();
        }

        return;
    }

    UpdateCollisions();

    _ball.Update(dt);
    _platform.Update(dt);
}

void Game::Draw(Display& display)
{
    if (_needClearDisplay)
    {
        display.FillBlack();
        _needClearDisplay = false;
    }

    if (_gameOverTimeOut > 0.f)
    {
        display.DrawImage(GameOver);
        display.UpdateScreen();
        return;
    }

    for (const auto& rect : _rectsToClear)
    {
        display.DrawRect(rect.x, rect.y, rect.w, rect.h, false);
    }

    _rectsToClear.clear();

    for (auto& brick : _bricks)
    {
        brick.Draw(display);
    }

    _ball.Draw(display);
    _platform.Draw(display);

    display.UpdateScreen();
}

void Game::OnLeftPressed()
{
    _platform.SetLeftPressed(true);
    _pressTimeOut = PRESS_TIMEOUT;
}

void Game::OnRightPressed()
{
    _platform.SetRightPressed(true);
    _pressTimeOut = PRESS_TIMEOUT;
}

void Game::UpdateCollisions()
{
    const Circle ballCircle = _ball.GetCircle();
    const Rect platformRect = _platform.GetRect();

    if (Intersects(ballCircle, platformRect))
    {
        const float t = 1.f - std::clamp((ballCircle.x - platformRect.x) / platformRect.w, 0.f, 1.f);
        const float angle = Lerp(BALL_MIN_ANGLE, BALL_MAX_ANGLE, t);
        const float velocityX = BALL_SPEED * std::cos(angle);
        const float velocityY = BALL_SPEED * std::sin(angle);
        _ball.SetVelocityX(velocityX);
        _ball.SetVelocityY(velocityY);
        _ball.SetYF(platformRect.y + platformRect.h + ballCircle.r);

        _platform.SetDirty(true);
    }
    else
    {
        if (IntersectsHorizontalWall(ballCircle, 0)) // bottom wall
        {
            _gameOverTimeOut = GAME_OVER_TIMEOUT;
            return;
        }

        if (IntersectsHorizontalWall(ballCircle, DISPLAY_HEIGHT - 1)) // top wall
        {
            _ball.SetVelocityY(-std::fabs(_ball.GetVelocityY()));
        }
        else if (IntersectsVerticalWall(ballCircle, DISPLAY_WIDTH - 1)) // right wall
        {
            _ball.SetVelocityX(-std::fabs(_ball.GetVelocityX()));
        }
        else if (IntersectsVerticalWall(_ball.GetCircle(), 0)) // left wall
        {
            _ball.SetVelocityX(std::fabs(_ball.GetVelocityX()));
        }
    }

    bool brickHit = false;

    for (size_t i = 0; i < _bricks.size(); ++i)
    {
        Brick& brick = _bricks[i];
        const Rect brickRect = brick.GetRect();
        const CollisionSide side = GetCollisionSide(ballCircle, brickRect);

        if (side != CollisionSide::None)
        {
            brick.SetDirty(true);
            if (!brickHit)
            {
                brickHit = true;
                brick.OnHit();
                _rectsToClear.push_back(brick.GetRectToClear());

                switch (side)
                {
                    case CollisionSide::Top:
                        _ball.SetVelocityY(std::fabs(_ball.GetVelocityY()));
                        _ball.SetYF(brickRect.y + brickRect.h + ballCircle.r);
                        break;
                    case CollisionSide::Bottom:
                        _ball.SetVelocityY(-std::fabs(_ball.GetVelocityY()));
                        _ball.SetYF(brickRect.y - ballCircle.r);
                        break;
                    case CollisionSide::Left:
                        _ball.SetVelocityX(-std::fabs(_ball.GetVelocityX()));
                        _ball.SetXF(brickRect.x - ballCircle.r);
                        break;
                    case CollisionSide::Right:
                        _ball.SetVelocityX(std::fabs(_ball.GetVelocityX()));
                        _ball.SetXF(brickRect.x + brickRect.w + ballCircle.r);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    if (brickHit)
    {
        _bricks.erase(std::remove_if(_bricks.begin(), _bricks.end(), [](const Brick& brick) {
            return brick.GetLevel() == 0;
        }), _bricks.end());
    }
}