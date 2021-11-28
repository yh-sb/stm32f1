add_library(periph STATIC
    src/adc.cpp
    src/dac.cpp
    src/dma.cpp
    src/exti.cpp
    src/gpio.cpp
    src/pwm.cpp
    src/rcc.cpp
    src/spi.cpp
    src/systick.cpp
    src/tim.cpp
    src/uart.cpp
    src/wdt.cpp
)

target_include_directories(periph
    PUBLIC
        include
)

target_link_libraries(periph
    PUBLIC
        freertos
    PRIVATE
        cmsis
)
