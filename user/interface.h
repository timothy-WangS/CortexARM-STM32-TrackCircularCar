#ifndef __INTERFACE_H_ 
#define __INTERFACE_H_ 
 
#include "stm32f10x.h" 
 
//user LED 
#define LED_PIN         GPIO_Pin_13 
#define LED_GPIO        GPIOC 
#define LED_SET         GPIO_SetBits(LED_GPIO , LED_PIN) 
#define LED_RESE        GPIO_ResetBits(LED_GPIO , LED_PIN) 
 
//电机驱动IO定义  
/*  
LEFT_F_PIN  PB10 左后退IO 
LEFT_B_PIN  PB11 左前进IO 
 
RIGHT_F_PIN  PB12 右前进IO 
RIGHT_B_PIN  PB13    右后退IO 
 
LEFT_EN_PIN  H-3.3v 左驱动使能 H有效 
RIGHT_EN_PIN H-3.3v 右驱动使能 H有效 
 */ 
#define LEFT_F_PIN      GPIO_Pin_11 
#define LEFT_F_GPIO     GPIOB 
#define LEFT_F_SET      GPIO_SetBits(LEFT_F_GPIO , LEFT_F_PIN) 
#define LEFT_F_RESET    GPIO_ResetBits(LEFT_F_GPIO , LEFT_F_PIN) 
 
#define LEFT_B_PIN      GPIO_Pin_10 
#define LEFT_B_GPIO     GPIOB 
#define LEFT_B_SET      GPIO_SetBits(LEFT_B_GPIO , LEFT_B_PIN) 
#define LEFT_B_RESET    GPIO_ResetBits(LEFT_B_GPIO , LEFT_B_PIN) 
 
#define RIGHT_F_PIN     GPIO_Pin_12 
#define RIGHT_F_GPIO    GPIOB 
#define RIGHT_F_SET     GPIO_SetBits(RIGHT_F_GPIO , RIGHT_F_PIN) 
#define RIGHT_F_RESET   GPIO_ResetBits(RIGHT_F_GPIO , RIGHT_F_PIN) 
 
#define RIGHT_B_PIN     GPIO_Pin_13 
#define RIGHT_B_GPIO    GPIOB 
#define RIGHT_B_SET     GPIO_SetBits(RIGHT_B_GPIO , RIGHT_B_PIN) 
#define RIGHT_B_RESET   GPIO_ResetBits(RIGHT_B_GPIO , RIGHT_B_PIN) 
 
//循迹光电对管 
/*  
 
左2循迹 SEARCH_L2_PIN PB5 
左1循迹 SEARCH_L1_PIN PB6 
中循迹 SEARCH_M_PIN PB7 
右1循迹 SEARCH_R1_PIN PB8 
右2循迹 SEARCH_R2_PIN PB9 
 */ 
#define SEARCH_L2_PIN        GPIO_Pin_5 
#define SEARCH_L2_GPIO       GPIOB 
#define SEARCH_L2_IO         GPIO_ReadInputDataBit(SEARCH_L2_GPIO, SEARCH_L2_PIN) 
 
#define SEARCH_L1_PIN        GPIO_Pin_6 
#define SEARCH_L1_GPIO       GPIOB 
#define SEARCH_L1_IO         GPIO_ReadInputDataBit(SEARCH_L1_GPIO, SEARCH_L1_PIN) 
 
#define SEARCH_M_PIN         GPIO_Pin_7 
#define SEARCH_M_GPIO        GPIOB 
#define SEARCH_M_IO          GPIO_ReadInputDataBit(SEARCH_M_GPIO, SEARCH_M_PIN) 
 
#define SEARCH_R1_PIN        GPIO_Pin_8 
#define SEARCH_R1_GPIO       GPIOB 
#define SEARCH_R1_IO         GPIO_ReadInputDataBit(SEARCH_R1_GPIO, SEARCH_R1_PIN) 
 
#define SEARCH_R2_PIN        GPIO_Pin_9 
#define SEARCH_R2_GPIO       GPIOB 
#define SEARCH_R2_IO         GPIO_ReadInputDataBit(SEARCH_R2_GPIO, SEARCH_R2_PIN) 
 
#define BLACK_AREA 1  //无信号返回 
#define WHITE_AREA 0  //有信号返回 
 
//测速 
/*  
速度码盘右 FRONT_RIGHT_S_PIN PA11 
速度码盘左 FRONT_LEFT_S_PIN PA12 
 */ 
#define RIGHT_S_PIN         GPIO_Pin_11 
#define RIGHT_S_GPIO        GPIOA 
#define RIGHT_S_IO          GPIO_ReadInputDataBit(RIGHT_S_GPIO, RIGHT_S_PIN) 
 
#define LEFT_S_PIN         GPIO_Pin_12 
#define LEFT_S_GPIO        GPIOA 
#define LEFT_S_IO          GPIO_ReadInputDataBit(LEFT_S_GPIO, LEFT_S_PIN) 
 
//左前 
#define LEFT_GO    LEFT_F_SET;   LEFT_B_RESET//前进 
 
#define LEFT_BACK  LEFT_F_RESET; LEFT_B_SET//后退 
#define LEFT_STOP  LEFT_F_RESET; LEFT_B_RESET//停止 
 
//右前 
#define RIGHT_GO     RIGHT_F_SET;  RIGHT_B_RESET 
#define RIGHT_BACK   RIGHT_F_RESET;RIGHT_B_SET 
#define RIGHT_STOP   RIGHT_F_RESET;RIGHT_B_RESET 
 
#define MAX_SPEED_DUTY 15 //默认占空比 按1ms最小分辨率 周期50ms计算 
#define MID_SPEED_DUTY 10 //默认占空比 按1ms最小分辨率 周期50ms计算 
#define MIN_SPEED_DUTY 0  //默认占空比 按1ms最小分辨率 周期50ms计算 
 
//指令定义 
#define COMM_STOP  'I'//停止 
#define COMM_UP    'A'//前进 
#define COMM_UPL   'X'//偏左前进 
#define COMM_UPR   'Y'//偏右前进 
#define COMM_UPLL  'P'//偏左前进 
#define COMM_UPRR  'Q'//偏右前进 
#define COMM_DOWN  'B'//后退 
#define COMM_LEFT  'C'//左转 
#define COMM_RIGHT 'D'//右转 
//#define COMM_TURNAROUND 'Z'//掉头(turn around) 
 
extern uint8_t tick_5ms;//5ms计数器，作为主函数的基本周期 
extern uint8_t tick_1ms;//1ms计数器，作为电机的基本计数器 
extern uint16_t speed_count;//占空比计数器 50次一周期 
 
#endif 
 

