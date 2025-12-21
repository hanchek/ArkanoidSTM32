extern "C"
{
#include "main.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "i2c.h"
}

#include "ssd1306/Display.h"
#include "Game/Game.h"

#include <vector>

int main()
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    Display display;
    display.Init(&hi2c1);

    Game game;
    game.Init();

    constexpr int framesCount = 64;
    uint32_t frames[framesCount];
    std::fill(std::begin(frames), std::end(frames), 0);
    int i = 0;

    uint32_t lastTick  = DWT->CYCCNT;

    while (true)
    {
        uint32_t dtCycles = DWT->CYCCNT - lastTick;
        lastTick += dtCycles;
        float dt = static_cast<float>(dtCycles) / static_cast<float>(SystemCoreClock);

        if (dt < 1.f / 60.f)
        {
            HAL_Delay(static_cast<uint32_t>((1.f / 60.f - dt) * 1000.f));
            dt = 1.f / 60.f;
        }

        game.Update(dt);
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2))
        {
            game.OnLeftPressed();
        }
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3))
        {
            game.OnRightPressed();
        }
        uint32_t start = DWT->CYCCNT;
        game.Draw(display);
        uint32_t cycleTime = DWT->CYCCNT - start;

        frames[i] = cycleTime / (SystemCoreClock / 1e6);
        ++i;
        if (i >= framesCount)
        {
            i = 0;
        }
    }

    return 0;
}
