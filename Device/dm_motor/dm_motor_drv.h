#ifndef __DM_MOTOR_DRV_H__
#define __DM_MOTOR_DRV_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "can.h"
#include "bsp_can.h"

#define MIT_MODE 0x000
#define POS_MODE 0x100
#define SPD_MODE 0x200
#define PSI_MODE 0x300

#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f

typedef enum
{
	Motor1,
	Motor2,
	Motor3,
	Motor4,
	Motor5,
	Motor6,
	Motor7,
	Motor8,
	Motor9,
	Motor10,
	motor_max_num
} MOTOR_NUM;

typedef enum
{
	mit_mode = 1,
	pos_mode = 2,
	spd_mode = 3,
	psi_mode = 4
} MODE_E;

// �������
typedef struct
{
	uint8_t read_flag;
	uint8_t write_flag;
	uint8_t save_flag;
	
	float    UV_Value;		// ��ѹ����ֵ
	float    KT_Value;		// Ť��ϵ��
	float    OT_Value;		// ���±���ֵ
	float    OC_Value;		// ��������ֵ
	float    ACC;			// ���ٶ�
	float    DEC;			// ���ٶ�
	float    MAX_SPD;		// ����ٶ�
	uint32_t MST_ID;	// ����ID
	uint32_t ESC_ID;	// ����ID
	uint32_t TIMEOUT;	// ��ʱ����ʱ��
	uint32_t cmode;		// ����ģʽ
	float  	 Damp;		// ���ճ��ϵ��
	float    Inertia;	// ���ת������
	uint32_t hw_ver;	// ����
	uint32_t sw_ver;	// ����汾��
	uint32_t SN;		// ����
	uint32_t NPP;		// ���������
	float    Rs;		// ����
	float    Ls;		// ���
	float    Flux;		// ����
	float    Gr;		// ���ּ��ٱ�
	float    PMAX;		// λ��ӳ�䷶Χ
	float    VMAX;		// �ٶ�ӳ�䷶Χ
	float    TMAX;		// Ť��ӳ�䷶Χ
	float    I_BW;		// ���������ƴ���
	float    KP_ASR;	// �ٶȻ�Kp
	float    KI_ASR;	// �ٶȻ�Ki
	float    KP_APR;	// λ�û�Kp
	float    KI_APR;	// λ�û�Ki
	float    OV_Value;	// ��ѹ����ֵ
	float    GREF;		// ��������Ч��
	float    Deta;		// �ٶȻ�����ϵ��
	float 	 V_BW;		// �ٶȻ��˲�����
	float 	 IQ_cl;		// ��������ǿϵ��
	float    VL_cl;		// �ٶȻ���ǿϵ��
	uint32_t can_br;	// CAN�����ʴ���
	uint32_t sub_ver;	// �Ӱ汾��
	
	float  u_off;		// u��ƫ��
	float	 v_off;		// v��ƫ��
	float	 k1;		  // ��������1
	float  k2;		  // ��������2
	float  m_off;		// �Ƕ�ƫ��
	float  dir;		  // ����
	float	 p_m;		  // ���λ��
	float	 x_out;		// �����λ��
} ESC_INFO_T;

// ����ش���Ϣ�ṹ��
typedef struct
{
	int   id;
	int   state;
	int   p_int;
	int   v_int;
	int   t_int;
	int   kp_int;
	int   kd_int;
	float pos;
	float vel;
	float tor;
	float Kp;
	float Kd;
	float Tmos;
	float Tcoil;
} MOTOR_STATE_T;

// ����������ýṹ��
typedef struct
{
	uint8_t mode;
	float pos_set;
	float vel_set;
	float tor_set;
	float cur_set;
	float kp_set;
	float kd_set;
} MOTOR_CTRL_T;

typedef struct
{
	uint16_t id;
	uint16_t mst_id;
	MOTOR_STATE_T para;
	MOTOR_CTRL_T ctrl;
	ESC_INFO_T tmp;
} MOTOR_T;


float uint_to_float(int x_int, float x_min, float x_max, int bits);
int float_to_uint(float x_float, float x_min, float x_max, int bits);
void dm_motor_ctrl_send(hcan_t* hcan, MOTOR_T *motor);
void dm_motor_enable(hcan_t* hcan, MOTOR_T *motor);
void dm_motor_disable(hcan_t* hcan, MOTOR_T *motor);
void dm_motor_clear_para(MOTOR_T *motor);
void dm_motor_clear_err(hcan_t* hcan, MOTOR_T *motor);
void dm_motor_fbdata(MOTOR_T *motor, uint8_t *rx_data);

void enable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);
void disable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);

void mit_ctrl(hcan_t* hcan, MOTOR_T *motor, uint16_t motor_id, float pos, float vel,float kp, float kd, float tor);
void pos_ctrl(hcan_t* hcan, uint16_t motor_id, float pos, float vel);
void spd_ctrl(hcan_t* hcan, uint16_t motor_id, float vel);
void psi_ctrl(hcan_t* hcan, uint16_t motor_id, float pos, float vel, float cur);
	
void save_pos_zero(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);
void clear_err(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);

void read_motor_data(uint16_t id, uint8_t rid);
void read_motor_ctrl_fbdata(uint16_t id);
void write_motor_data(uint16_t id, uint8_t rid, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
void save_motor_data(uint16_t id, uint8_t rid);

#ifdef __cplusplus
 }
#endif

#endif /* __DM_MOTOR_DRV_H__ */

