cmake_minimum_required(VERSION 3.20)
project(STM32F446RE C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(STM32_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stm32)
set(LINKER_SCRIPT ${STM32_DIR}/STM32F446XX_FLASH.ld)

# Include paths
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${STM32_DIR}/Core/Inc
    ${STM32_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${STM32_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${STM32_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${STM32_DIR}/Drivers/CMSIS/Include
)

# HAL C files
file(GLOB_RECURSE C_SOURCES
    ${STM32_DIR}/Core/Src/*.c
    ${STM32_DIR}/Drivers/**/*.c
)

# Your C++ sources
file(GLOB_RECURSE CPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME}
    ${C_SOURCES}
    ${CPP_SOURCES}
    ${STM32_DIR}/startup_stm32f446xx.s
)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

# MCU settings
target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -ffunction-sections
    -fdata-sections
)

target_link_options(${PROJECT_NAME} PRIVATE
    "-T${LINKER_SCRIPT}"
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
)

# Create HEX & BIN
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)

add_compile_definitions(STM32F446xx)
