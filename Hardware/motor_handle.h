#ifndef __MOTOR_HANDLE_H__
#define __MOTOR_HANDLE_H__

#include "include.h"
#include "pwm.h"

#define MOTOR_DIR_CHANGE_DLY_TIME ((u16)50)                       // 电机切换方向时，中间停止转动的时间，单位：ms
#define MOTOR_WORKING_PWM_DUTY_VAL ((u16)PWM_DUTY_VAL_85_PERCENT) // 电机工作时，对应的PWM占空比值
#define MOTOR_OVERCURRENT_ADC_VAL ((u16)1657)                     // 电机过流时，adc检测对应的阈值（约1.7V）
#define MOTOR_OVERCURRENT_TIME_CNT_MAX ((u16)200)                 // 电机过流检测的计数值最大值
/*
    电机工作正常时，超时时间

    8000： 客户实际测试是10秒左右
    5000： 客户实际测试是6秒左右，需要减少1500ms

    8000：10秒
    5000：6秒

    假设y为秒，x是数值
    根据 y = kx + b，列出两个等式
    10 = 8000k + b
    6 = 5000k + b

    4 = 3000k
    k = 4/3000 约为 0.00133
    b = 10 - 8000 * 0.00133 = -0.64

    如果要 y = 5,则 x = (5 + 0.64) / 0.00133 = 4,240
*/
// #define MOTOR_WORKING_OVER_TIME ((u16)5000)
#define MOTOR_WORKING_OVER_TIME ((u16)4240)

#define MOTOR_0_FORWARD_PWM_DUTY_SET(motor_0_pwm_duty) pwm_channel_0_set_duty(motor_0_pwm_duty)
#define MOTOR_0_REVERSE_PWM_DUTY_SET(motor_0_pwm_duty) pwm_channel_1_set_duty(motor_0_pwm_duty)

// 定义电机状态
enum
{
    MOTOR_STATUS_STOP = 0, // 电机停止工作
    MOTOR_STATUS_FORWARD,  // 电机正在正转
    MOTOR_STATUS_REVERSE,
};
typedef u8 motor_status_t;

/*
    电机方向，
    在dest_dir中，作为矢量来使用，并不代表电机当前的状态
*/
enum
{
    MOTOR_DIR_NONE = 0, // 默认状态
    MOTOR_DIR_FORWARD,
    MOTOR_DIR_REVERSE,
};
typedef u8 motor_dest_dir_t;

typedef struct
{
    motor_dest_dir_t dest_dir; // 目标方向（除了刚上电，其余情况只能是正转或者反转）
    motor_status_t status;     // 当前状态

    u8 change_dir_enable; // 是否要改变方向
    u16 change_dir_cnt;   // 改变方向的计数器（电机不能马上改变方向，如果同时正转和反转，驱动电机的MOS管会短路）

    u16 overcurrent_time_cnt; // 过流检测计数器
    u16 adc_val_forward;      // 正转时，检测到的ad值
    u16 adc_val_reverse;      // 反转时，检测到的ad值

    u16 working_time_cnt; // 工作时间计数器

    void (*adc_val_update)(void); // 更新ad值
    void (*forward)(void);        // 正转
    void (*reverse)(void);        // 反转
    void (*stop)(void);           // 停止

} motor_handle_t;

extern volatile motor_handle_t motor_handle_0;
// extern volatile motor_handle_t motor_handle_1;

void motor_handle_init(void);
void motor_handle_func_0(void);

// motor_handle_x 的友元函数
// 设置电机要切换到的转动方向
void motor_set_dir(motor_handle_t *motor_handle, motor_dest_dir_t dest_dir);

#endif
