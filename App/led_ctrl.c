#include "main.h"

#include "cmsis_os.h"

void StartDefaultTask(void* argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(100);
    }
    /* USER CODE END StartDefaultTask */
}
