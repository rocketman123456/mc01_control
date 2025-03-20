#include "main.h"

#include "cmsis_os.h"

#include "adc.h"
#include "state_data.h"
#include "usbd_cdc_if.h"

volatile float voltage_vrefint_proportion = 3.3 / 4096.0; // 8.0586080586080586080586080586081e-4f;// 3.3/4096

static uint16_t adcx_get_chx_value(ADC_HandleTypeDef* ADCx, uint32_t ch)
{
    static ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel                       = ch;
    sConfig.Rank                          = 1;
    sConfig.SamplingTime                  = ADC_SAMPLETIME_3CYCLES; // ADC_SAMPLETIME_3CYCLES;

    if (HAL_ADC_ConfigChannel(ADCx, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_ADC_Start(ADCx);

    HAL_ADC_PollForConversion(ADCx, 10);
    return (uint16_t)HAL_ADC_GetValue(ADCx);
}

static float get_battery_voltage(void)
{
    float    volt;
    uint16_t adcx = 0;

    adcx = adcx_get_chx_value(&hadc1, ADC_CHANNEL_0);
    //(10K + 100K)  / 10K = 11
    volt = (float)adcx * voltage_vrefint_proportion * 11.0f;

    return volt;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    /* Prevent unused argument(s) compilation warning */
    voltage = HAL_ADC_GetValue(hadc);
    voltage = voltage * 3.3f / 4096.0f * 11.0f;
}

void VoltReadTask(void* argument)
{
    /* USER CODE BEGIN VoltReadTask */
    // HAL_ADC_Start_IT(&hadc1);
    /* Infinite loop */
    for (;;)
    {
        voltage = get_battery_voltage();
        // sprintf((char*)usb_tx_buffer, "voltage %4f \r\n", voltage);
        // CDC_Transmit_FS(usb_tx_buffer, strlen((char*)usb_tx_buffer));
        osDelay(1000);
    }
    /* USER CODE END VoltReadTask */
}
