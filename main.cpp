// Example for STM32VLDISCOVERY development board

#include "periph/gpio.hpp"
#include "periph/systick.hpp"
#include "FreeRTOS.h"
#include "task.h"

static void heartbeat_task(void *pvParameters)
{
    periph::gpio *green_led = (periph::gpio *)pvParameters;
    while(1)
    {
        green_led->toggle();
        vTaskDelay(500);
    }
}

int main(void)
{
    periph::systick::init();
    static periph::gpio green_led(2, 9, periph::gpio::mode::DO, 0);
    
    xTaskCreate(heartbeat_task, "heartbeat", configMINIMAL_STACK_SIZE,
        &green_led, 1, nullptr);
    
    vTaskStartScheduler();
}
