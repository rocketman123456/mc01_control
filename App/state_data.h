#ifndef __STATE_DATA_H
#define __STATE_DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

    typedef enum
    {
        MOTOR_CMD_ENABLE   = 0,
        MOTOR_CMD_DISABLE  = 1,
        MOTOR_CMD_SET_ZERO = 2,
        MOTOR_CMD_CONTROL  = 3,
    } MotorCmd;

#pragma pack(1)
    typedef struct
    {
        uint16_t id;
        uint16_t mst_id;
        float    pos;
        float    vel;
        float    tor;
    } GlobalMotorState;

    typedef struct
    {
        uint16_t id;
        uint16_t mst_id;
        float    pos_set;
        float    vel_set;
        float    tor_set;
        float    kp_set;
        float    kd_set;
        uint8_t  cmd;
    } GlobalMotorCmd;
#pragma pack()

    extern GlobalMotorState motor_states[6];
    extern GlobalMotorCmd   motor_cmds[6];

    extern uint8_t usb_rx_buffer[64];
    extern uint8_t usb_tx_buffer[64];

    extern float voltage;

    extern uint16_t index_to_id[16];
    extern uint16_t id_to_index[17];

#ifdef __cplusplus
}
#endif

#endif
