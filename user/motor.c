#include "motor.h" 
#include "interface.h" 
#include "stm32f10x.h" 
 
//GPIO���ú��� 
void MotorGPIO_Configuration(void) 
{   
 GPIO_InitTypeDef GPIO_InitStructure; 
 
 GPIO_InitStructure.GPIO_Pin = LEFT_F_PIN | LEFT_B_PIN | RIGHT_F_PIN | 
RIGHT_B_PIN; 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
 GPIO_Init(LEFT_F_GPIO, &GPIO_InitStructure);     
} 
 
//����ռ�ձ��������ת�� 
void CarMove(void) 
{    
 //���� 
 if(left_speed_duty > 0)//��ǰ 
 { 
  if(speed_count < left_speed_duty) 
  { 
   LEFT_GO; 
  }else 
  { 
   LEFT_STOP; 
  } 
 } 
 else if(left_speed_duty < 0)//��� 
 { 
  if(speed_count < (-1)*left_speed_duty) 
  { 
   LEFT_BACK; 
  }else 
  { 
   LEFT_STOP; 
  } 
 } 
 else                //ֹͣ 
 { 
  LEFT_STOP; 
 } 
 
  
 //���� 
 if(right_speed_duty > 0)//��ǰ 
 { 
  if(speed_count < right_speed_duty) 
  { 
   RIGHT_GO; 
  }else                //ֹͣ 
  { 
   RIGHT_STOP; 
  } 
 } 
 else if(right_speed_duty < 0)//��� 
 { 
  if(speed_count < (-1)*right_speed_duty) 
  { 
   RIGHT_BACK; 
  }else                //ֹͣ 
  { 
   RIGHT_STOP; 
  } 
 } 
 else                //ֹͣ 
 { 
  RIGHT_STOP; 
 } 
} 
 
//��ǰ 
void CarGo(void) 
{ 
 left_speed_duty=MAX_SPEED_DUTY; 
 right_speed_duty=MAX_SPEED_DUTY; 
} 
//��������ǰ 
void CarGoL(void) 
{ 
 left_speed_duty=MAX_SPEED_DUTY; 
 right_speed_duty=MAX_SPEED_DUTY-10; 
} 
//��������ǰ 
void CarGoR(void) 
{ 
 left_speed_duty=MAX_SPEED_DUTY-10; 
 
 right_speed_duty=MAX_SPEED_DUTY; 
} 
 
//���� 
void CarLeft(void) 
{ 
 left_speed_duty=-MID_SPEED_DUTY; 
 right_speed_duty=MID_SPEED_DUTY; 
} 
 
//���� 
void CarRight(void) 
{ 
 left_speed_duty=MID_SPEED_DUTY; 
 right_speed_duty=-MID_SPEED_DUTY; 
} 
 
//ֹͣ 
void CarStop(void) 
{ 
 left_speed_duty=MIN_SPEED_DUTY; 
 right_speed_duty=MIN_SPEED_DUTY; 
} 
 
//���� 
//void CarBack(void) 
//{ 
// left_speed_duty=-SPEED_DUTY; 
// right_speed_duty=-SPEED_DUTY; 
//} 
 
void MotorInit(void) 
{ 
 MotorGPIO_Configuration(); 
 CarStop(); 
} 

