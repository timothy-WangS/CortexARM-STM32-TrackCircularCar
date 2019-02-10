#include "motor.h" 
#include "interface.h" 
#include "stm32f10x.h" 
 
//GPIO配置函数 
void MotorGPIO_Configuration(void) 
{   
 GPIO_InitTypeDef GPIO_InitStructure; 
 
 GPIO_InitStructure.GPIO_Pin = LEFT_F_PIN | LEFT_B_PIN | RIGHT_F_PIN | 
RIGHT_B_PIN; 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
 GPIO_Init(LEFT_F_GPIO, &GPIO_InitStructure);     
} 
 
//根据占空比驱动电机转动 
void CarMove(void) 
{    
 //左轮 
 if(left_speed_duty > 0)//向前 
 { 
  if(speed_count < left_speed_duty) 
  { 
   LEFT_GO; 
  }else 
  { 
   LEFT_STOP; 
  } 
 } 
 else if(left_speed_duty < 0)//向后 
 { 
  if(speed_count < (-1)*left_speed_duty) 
  { 
   LEFT_BACK; 
  }else 
  { 
   LEFT_STOP; 
  } 
 } 
 else                //停止 
 { 
  LEFT_STOP; 
 } 
 
  
 //右轮 
 if(right_speed_duty > 0)//向前 
 { 
  if(speed_count < right_speed_duty) 
  { 
   RIGHT_GO; 
  }else                //停止 
  { 
   RIGHT_STOP; 
  } 
 } 
 else if(right_speed_duty < 0)//向后 
 { 
  if(speed_count < (-1)*right_speed_duty) 
  { 
   RIGHT_BACK; 
  }else                //停止 
  { 
   RIGHT_STOP; 
  } 
 } 
 else                //停止 
 { 
  RIGHT_STOP; 
 } 
} 
 
//向前 
void CarGo(void) 
{ 
 left_speed_duty=MAX_SPEED_DUTY; 
 right_speed_duty=MAX_SPEED_DUTY; 
} 
//需向右向前 
void CarGoL(void) 
{ 
 left_speed_duty=MAX_SPEED_DUTY; 
 right_speed_duty=MAX_SPEED_DUTY-10; 
} 
//需向左向前 
void CarGoR(void) 
{ 
 left_speed_duty=MAX_SPEED_DUTY-10; 
 
 right_speed_duty=MAX_SPEED_DUTY; 
} 
 
//向左 
void CarLeft(void) 
{ 
 left_speed_duty=-MID_SPEED_DUTY; 
 right_speed_duty=MID_SPEED_DUTY; 
} 
 
//向右 
void CarRight(void) 
{ 
 left_speed_duty=MID_SPEED_DUTY; 
 right_speed_duty=-MID_SPEED_DUTY; 
} 
 
//停止 
void CarStop(void) 
{ 
 left_speed_duty=MIN_SPEED_DUTY; 
 right_speed_duty=MIN_SPEED_DUTY; 
} 
 
//后退 
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

