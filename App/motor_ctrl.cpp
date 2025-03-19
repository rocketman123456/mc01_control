#include "main.h"

#include "cmsis_os.h"

#include "math.h"
#include "tim.h"

// dm motor
#include "bsp_can.h"
#include "dm_motor_ctrl.h"
#include "dm_motor_drv.h"

// rs motor
#include "robstrite.h"

extern osSemaphoreId_t can1BinarySemHandle;
extern osSemaphoreId_t can2BinarySemHandle;
extern osMutexId_t     motorDataMutexHandle;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

RobStrite_Motor RobStrite_01(0x02, &hcan2);
RobStrite_Motor RobStrite_02(0x04, &hcan2);

uint16_t motor_ids[] = {Motor1, Motor2};

#ifdef __cplusplus
extern "C"
{
#endif

    void CAN1_CtrlTask(void* argument)
    {
        /* USER CODE BEGIN CAN1_CtrlTask */
        osDelay(1000);
        // write_motor_data(motor[Motor1].id, 10, 1, 0, 0, 0);
        // osDelay(1);
        // save_motor_data(motor[Motor1].id, 10);
        // osDelay(1);
        for (int i = 0; i < 2; ++i)
        {
            dm_motor_enable(&hcan1, &motor[motor_ids[i]]);
            osDelay(1);
            dm_motor_enable(&hcan1, &motor[motor_ids[i]]);
            osDelay(1);
        }
        osDelay(1000);
        // HAL_TIM_Base_Start_IT(&htim3);
        /* Infinite loop */
        float phase = 0.0;
        for (;;)
        {
            osSemaphoreAcquire(can1BinarySemHandle, osWaitForever);

            phase += 0.01f;

            for (int i = 0; i < 2; ++i)
            {
                motor[motor_ids[i]].ctrl.pos_set = sin(phase);
                motor[motor_ids[i]].ctrl.vel_set = 0.0;
                motor[motor_ids[i]].ctrl.kp_set  = 2.0;
                motor[motor_ids[i]].ctrl.kd_set  = 0.5;
                motor[motor_ids[i]].ctrl.tor_set = 0.0;
                mit_ctrl(
                    &hcan1,
                    &motor[motor_ids[i]],
                    motor[motor_ids[i]].id,
                    motor[motor_ids[i]].ctrl.pos_set,
                    motor[motor_ids[i]].ctrl.vel_set,
                    motor[motor_ids[i]].ctrl.kp_set,
                    motor[motor_ids[i]].ctrl.kd_set,
                    motor[motor_ids[i]].ctrl.tor_set
                );
            }
        }
        /* USER CODE END CAN1_CtrlTask */
    }

    void CAN2_CtrlTask(void* argument)
    {
        /* USER CODE BEGIN CAN2_CtrlTask */
        osDelay(1000);
        // RobStrite_01.RobStrite_Get_CAN_ID();  //�����ȡ�豸ID��MCU���밴����Ҫʹ��
        // osDelay(1);
        // RobStrite_01.Set_CAN_ID(0x02);				//�������CANID���밴����Ҫʹ��
        // osDelay(1);
        // RobStrite_01.Set_ZeroPos();					  //���õ�ǰλ��Ϊ�����е��λ���밴����Ҫʹ��
        RobStrite_01.Enable_Motor(); // ���ʹ��
        RobStrite_02.Enable_Motor();
        osDelay(1);
        RobStrite_01.Enable_Motor();
        RobStrite_02.Enable_Motor();
        osDelay(1);
        osDelay(1000);
        HAL_TIM_Base_Start_IT(&htim3);
        /* Infinite loop */
        float phase = 0.0;
        for (;;)
        {
            osSemaphoreAcquire(can2BinarySemHandle, osWaitForever);

            phase += 0.01f;

            //*********�˿�ģʽ*************//
            float T             = 0.0;        // Ť��
            float Angle         = sin(phase); // Ҫת����λ��
            float Speed         = sin(phase); // Ŀ���ٶ�
            float Pacceleration = 1.0;        // ���ٶ�
            float Kp            = 1.0;        // Kp
            float Kd            = 0.5;        // Kd
            RobStrite_01.RobStrite_Motor_move_control(T, Angle, Speed, Kp, Kd);
            RobStrite_02.RobStrite_Motor_move_control(T, -Angle, -Speed, Kp, Kd);

            // RobStrite_01.Disenable_Motor(0);	//���ʧ�ܣ�����ʹ��
        }
        /* USER CODE END CAN2_CtrlTask */
    }

    /**
    ************************************************************************
    * @brief:      	fdcan1_rx_callback: CAN1���ջص�����
    * @param:      	void
    * @retval:     	void
    * @details:    	����CAN1�����жϻص������ݽ��յ���ID�����ݣ�ִ����Ӧ�Ĵ���
    *               �����յ�IDΪ0ʱ������dm4310_fbdata��������Motor�ķ������ݡ�
    ************************************************************************
    **/
    void can1_rx_callback(void)
    {
        uint16_t rec_id;
        uint8_t  rx_data[8] = {0};
        canx_receive(&hcan1, &rec_id, rx_data);
        switch (rec_id)
        {
            case 0x81:
                dm_motor_fbdata(&motor[Motor1], rx_data);
                receive_motor_data(&motor[Motor1], rx_data);
                break;
            case 0x83:
                dm_motor_fbdata(&motor[Motor2], rx_data);
                receive_motor_data(&motor[Motor2], rx_data);
                break;
        }
    }

    void can2_rx_callback(void)
    {
        CAN_RxHeaderTypeDef rx_header;
        uint8_t             rx_data[8] = {0};
        uint8_t             rec_id;
        if (HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK)
        {
            if (rx_header.IDE == CAN_ID_EXT)
            {
                rec_id = uint8_t((rx_header.ExtId & 0xFF00) >> 8);
                if (rec_id == 0x02)
                {
                    RobStrite_01.RobStrite_Motor_Analysis(rx_data, rx_header.ExtId);
                }
                if (rec_id == 0x04)
                {
                    RobStrite_02.RobStrite_Motor_Analysis(rx_data, rx_header.ExtId);
                }
            }
        }
    }

#ifdef __cplusplus
}
#endif
