#ifndef __DM_MOTOR_CTRL_H__
#define __DM_MOTOR_CTRL_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "dm_motor_drv.h"

extern int8_t motor_id;

extern uint32_t motor1_data_sent;
extern uint32_t motor2_data_sent;
extern uint32_t motor3_data_sent;
extern uint32_t motor4_data_sent;

extern MOTOR_T motor[motor_max_num];

typedef union
{
	float f_val;
	uint32_t u_val;
	uint8_t b_val[4];
}float_type_u;

void dm_motor_init(void);

void read_all_motor_data(MOTOR_T *motor);
void receive_motor_data(MOTOR_T *motor, uint8_t *data);

#ifdef __cplusplus
 }
#endif

#endif /* __DM_MOTOR_CTRL_H__ */

