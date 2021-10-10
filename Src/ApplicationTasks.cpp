#include <main.h>

#include <FreeRTOS.h>
#include <task.h>

static StaticTask_t blinkTask;
static StackType_t blinkTaskStack[64];
void blinkTaskFunc(void *param);

extern "C"
{
    void startUserTasks()
    {
        xTaskCreateStatic(blinkTaskFunc, "blink", 64, NULL, 1, blinkTaskStack, &blinkTask);
    }
}

static volatile uint32_t blinkCounter = 1;
void blinkTaskFunc(void *param)
{
    while (1)
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        blinkCounter++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}