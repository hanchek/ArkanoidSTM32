#include "Game.h"

#include "GameOver.h"

#include <algorithm>
#include <cmath>

constexpr float PRESS_TIMEOUT = 0.03f;
constexpr float GAME_OVER_TIMEOUT = 1.f;
constexpr uint8_t BRICK_WIDTH = 15;
constexpr uint8_t BRICK_HEIGHT = 7;


void Game::Init()
{
    constexpr int columns = DISPLAY_WIDTH / (BRICK_WIDTH + 1);
    constexpr int rows = 3;

    for (int i = 0; i < columns; ++i)
    {
        for (int j = 0; j <= rows; ++j)
        {
            const uint8_t x = i * (BRICK_WIDTH + 1);
            const uint8_t y = DISPLAY_HEIGHT - j * (BRICK_HEIGHT + 1);
            _bricks.push_back(Brick(x, y, BRICK_WIDTH, BRICK_HEIGHT));
        }
    }

    _ball = Ball();
    _ball.SetVelocityX(60.f);
    _ball.SetVelocityY(60.f);
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

    if (Intersects(ballCircle, _platform.GetRect()))
    {
        _ball.SetVelocityY(std::fabs(_ball.GetVelocityY()));
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

    bool brickCollided = false;

    for (auto& brick : _bricks)
    {
        const Rect brickRect = brick.GetRect();
        const CollisionSide side = GetCollisionSide(ballCircle, brickRect);

        if (side != CollisionSide::None)
        {
            brick.SetDirty(true);
            if (!brickCollided)
            {
                brickCollided = true;
                _rectsToClear.push_back(brickRect);
                _bricks.erase(std::remove(_bricks.begin(), _bricks.end(), brick), _bricks.end());

                switch (side)
                {
                    case CollisionSide::Top:
                        _ball.SetVelocityY(std::fabs(_ball.GetVelocityY()));
                        break;
                    case CollisionSide::Bottom:
                        _ball.SetVelocityY(-std::fabs(_ball.GetVelocityY()));
                        break;
                    case CollisionSide::Left:
                        _ball.SetVelocityX(-std::fabs(_ball.GetVelocityX()));
                        break;
                    case CollisionSide::Right:
                        _ball.SetVelocityX(std::fabs(_ball.GetVelocityX()));
                        break;
                    default:
                        break;
                }
            }
        }
    }
}