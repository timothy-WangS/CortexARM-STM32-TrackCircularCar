#ifndef __INTERFACE_H_ 
#define __INTERFACE_H_ 
 
#include "stm32f10x.h" 
 
//user LED 
#define LED_PIN         GPIO_Pin_13 
#define LED_GPIO        GPIOC 
#define LED_SET         GPIO_SetBits(LED_GPIO , LED_PIN) 
#define LED_RESE        GPIO_ResetBits(LED_GPIO , LED_PIN) 
 
//�������IO����  
/*  
LEFT_F_PIN  PB10 �����IO 
LEFT_B_PIN  PB11 ��ǰ��IO 
 
RIGHT_F_PIN  PB12 ��ǰ��IO 
RIGHT_B_PIN  PB13    �Һ���IO 
 
LEFT_EN_PIN  H-3.3v ������ʹ�� H��Ч 
RIGHT_EN_PIN H-3.3v ������ʹ�� H��Ч 
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
 
//ѭ�����Թ� 
/*  
 
��2ѭ�� SEARCH_L2_PIN PB5 
��1ѭ�� SEARCH_L1_PIN PB6 
��ѭ�� SEARCH_M_PIN PB7 
��1ѭ�� SEARCH_R1_PIN PB8 
��2ѭ�� SEARCH_R2_PIN PB9 
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
 
#define BLACK_AREA 1  //���źŷ��� 
#define WHITE_AREA 0  //���źŷ��� 
 
//���� 
/*  
�ٶ������� FRONT_RIGHT_S_PIN PA11 
�ٶ������� FRONT_LEFT_S_PIN PA12 
 */ 
#define RIGHT_S_PIN         GPIO_Pin_11 
#define RIGHT_S_GPIO        GPIOA 
#define RIGHT_S_IO          GPIO_ReadInputDataBit(RIGHT_S_GPIO, RIGHT_S_PIN) 
 
#define LEFT_S_PIN         GPIO_Pin_12 
#define LEFT_S_GPIO        GPIOA 
#define LEFT_S_IO          GPIO_ReadInputDataBit(LEFT_S_GPIO, LEFT_S_PIN) 
 
//��ǰ 
#define LEFT_GO    LEFT_F_SET;   LEFT_B_RESET//ǰ�� 
 
#define LEFT_BACK  LEFT_F_RESET; LEFT_B_SET//���� 
#define LEFT_STOP  LEFT_F_RESET; LEFT_B_RESET//ֹͣ 
 
//��ǰ 
#define RIGHT_GO     RIGHT_F_SET;  RIGHT_B_RESET 
#define RIGHT_BACK   RIGHT_F_RESET;RIGHT_B_SET 
#define RIGHT_STOP   RIGHT_F_RESET;RIGHT_B_RESET 
 
#define MAX_SPEED_DUTY 15 //Ĭ��ռ�ձ� ��1ms��С�ֱ��� ����50ms���� 
#define MID_SPEED_DUTY 10 //Ĭ��ռ�ձ� ��1ms��С�ֱ��� ����50ms���� 
#define MIN_SPEED_DUTY 0  //Ĭ��ռ�ձ� ��1ms��С�ֱ��� ����50ms���� 
 
//ָ��� 
#define COMM_STOP  'I'//ֹͣ 
#define COMM_UP    'A'//ǰ�� 
#define COMM_UPL   'X'//ƫ��ǰ�� 
#define COMM_UPR   'Y'//ƫ��ǰ�� 
#define COMM_UPLL  'P'//ƫ��ǰ�� 
#define COMM_UPRR  'Q'//ƫ��ǰ�� 
#define COMM_DOWN  'B'//���� 
#define COMM_LEFT  'C'//��ת 
#define COMM_RIGHT 'D'//��ת 
//#define COMM_TURNAROUND 'Z'//��ͷ(turn around) 
 
extern uint8_t tick_5ms;//5ms����������Ϊ�������Ļ������� 
extern uint8_t tick_1ms;//1ms����������Ϊ����Ļ��������� 
extern uint16_t speed_count;//ռ�ձȼ����� 50��һ���� 
 
#endif 
 

