#include "main.h"

#include "cmsis_os.h"

#include "usbd_cdc_if.h"

extern osMutexId_t motorDataMutexHandle;

void USB_Task(void* argument)
{
    /* USER CODE BEGIN USB_Task */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END USB_Task */
}
