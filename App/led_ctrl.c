#include "main.h"

#include "cmsis_os.h"

#include "state_data.h"

void StartDefaultTask(void* argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        // basic task to check system state
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(1000);
    }
    /* USER CODE END StartDefaultTask */
}
