#include "main.h"

#include "cmsis_os.h"

#include "state_data.h"
#include "stdint.h"

GlobalMotorState motor_states[6];
GlobalMotorCmd   motor_cmds[6];

uint8_t usb_rx_buffer[64];
uint8_t usb_tx_buffer[64];

float voltage;

uint16_t index_to_id[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
uint16_t id_to_index[17] = {-1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
