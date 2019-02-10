#ifndef __MOTOR_H_ 
#define __MOTOR_H_ 
#include "stm32f10x.h" 
 
extern uint16_t speed_count;//ռ�ձȼ����� 50��һ���� 
extern int8_t left_speed_duty; 
extern int8_t right_speed_duty; 
 
void CarMove(void); 
void CarGo(void); 
void CarGoL(void); 
void CarGoR(void); 
//void CarBack(void); 
void CarLeft(void); 
void CarRight(void); 
void CarStop(void); 
void MotorInit(void); 
#endif 
 

