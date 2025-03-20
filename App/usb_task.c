#include "main.h"

#include "cmsis_os.h"
#include "queue.h"

#include "state_data.h"
#include "stdint.h"
#include "string.h"
#include "usbd_cdc_if.h"

extern osMutexId_t        motorDataMutexHandle;
extern osMessageQueueId_t usbCmdQueue1Handle;
extern osMessageQueueId_t usbCmdQueue2Handle;
extern osMessageQueueId_t usbStateQueueHandle;
extern osSemaphoreId_t    usbBinarySemHandle;

extern uint8_t usb_rx_buffer[64];
extern uint8_t usb_tx_buffer[64];

extern uint16_t index_to_id[16];
extern uint16_t id_to_index[17];

osMessageQueueId_t* cmd_queue_ids[4] = {&usbCmdQueue1Handle, &usbCmdQueue1Handle, &usbCmdQueue2Handle, &usbCmdQueue2Handle};

uint16_t get_id(uint8_t* buf) { return ((uint16_t)buf[0]) | ((uint16_t)buf[1] << 8); }

void decode_motor_cmd(uint8_t* buf, GlobalMotorCmd* cmd)
{
    static uint16_t len = sizeof(GlobalMotorCmd);

    uint8_t crc = buf[len]; // TODO : check crc
    memcpy(cmd, buf, len);
}

void pack_motor_cmd(uint8_t* buf, GlobalMotorCmd* state)
{
    static uint16_t len = sizeof(GlobalMotorCmd);

    memcpy(buf, state, len);
    buf[len] = 0x01; // TODO : crc
}

void decode_motor_state(uint8_t* buf, GlobalMotorState* cmd)
{
    static uint16_t len = sizeof(GlobalMotorState);

    uint8_t crc = buf[len]; // TODO : check crc
    memcpy(cmd, buf, len);
}

void pack_motor_state(uint8_t* buf, GlobalMotorState* state)
{
    static uint16_t len = sizeof(GlobalMotorState);

    memcpy(buf, state, len);
    buf[len] = 0x01; // TODO : crc
}

void USB_Task(void* argument)
{
    /* USER CODE BEGIN USB_Task */
    static GlobalMotorCmd   motor_cmd;
    static GlobalMotorState motor_state;

    osDelay(2000);

    uint16_t send   = 0;
    uint16_t id     = 0;
    uint16_t index  = 0;
    uint32_t result = 0;
    /* Infinite loop */
    for (;;)
    {
        // get motor states message
        // TODO : Update motor states
        uint32_t count = osMessageQueueGetCount(usbStateQueueHandle);
        if (count > 0)
        {
            for (int i = 0; i < count; ++i)
            {
                result = osMessageQueueGet(usbStateQueueHandle, &motor_state, NULL, 0);
                if (result == osOK)
                {
                    id    = motor_state.id;
                    index = id_to_index[id];

                    memcpy(&motor_states[index], &motor_state, sizeof(GlobalMotorState));
                }
            }
        }

        // check if pc gives command
        result = osSemaphoreAcquire(usbBinarySemHandle, 0);
        if (result == osOK)
        {
            // decode command, send to motor control task
            decode_motor_cmd(usb_rx_buffer, &motor_cmd);

            id    = motor_cmd.id;
            index = id_to_index[id];

            osMessageQueuePut(*cmd_queue_ids[index], &motor_cmd, 0, 0);

            pack_motor_state(usb_tx_buffer, &motor_states[index]);
            CDC_Transmit_FS(usb_tx_buffer, sizeof(GlobalMotorState) + 1);
        }
    }
    /* USER CODE END USB_Task */
}
