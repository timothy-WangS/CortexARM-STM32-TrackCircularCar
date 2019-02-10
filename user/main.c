#include "stm32f10x.h" 
#include "motor.h" 
#include "interface.h" 
#include "string.h" 
 
//全局变量定义 
uint16_t speed_count=0;//占空比计数器 50次一周期 
int8_t left_speed_duty=MIN_SPEED_DUTY; 
int8_t right_speed_duty=MIN_SPEED_DUTY; 
 
uint8_t tick_5ms = 0;//5ms计数器，作为主函数的基本周期 
uint8_t tick_1ms = 0;//1ms计数器，作为电机的基本计数器 
 
int8_t ctrl_comm = COMM_STOP;//控制指令 
int8_t ctrl_comm_last = COMM_STOP;//上一次的指令 
uint8_t continue_time=0; 
 
uint16_t stopCountR=0,stopCountL=0,brCount=0,noTurnRight=0; 
//no -此参数=0前,禁止右转.无此参数,左转结束,会立即进入右转状态. 
//br -右转90时,无此参数会立即停止转动. 
//stopL(R) -左(右)进入黑线的时间,短表示是十字或T,长是终点 
 
void RedRayInit(void); 
void GPIO_Conf(void); 
void RCC_Conf(void); 
void TIM2_Init(void); 
void MotorInit(void); 
void SEARCHPath(void); 
 
int main(void) 
{ 
 uint16_t i,j; 
  
 RCC_Conf(); 
 GPIO_Conf(); 
 RedRayInit(); 
 TIM2_Init(); 
 MotorInit(); 
 for(i=0;i<10000;i++) 
  for(j=0;j<1000;j++); 
 while (1) 
 { 
   if(tick_5ms >= 5) 
  { 
 
   tick_5ms = 0; 
   if(brCount > 0) 
   { 
    brCount--; 
   } 
   if(noTurnRight > 0) 
    noTurnRight--; 
   //do something 
   SEARCHPath(); //SearchRun(); 
   if(ctrl_comm_last != ctrl_comm)//指令发生变化 
   { 
    ctrl_comm_last = ctrl_comm; 
    switch(ctrl_comm) 
    { 
     case COMM_UP:    CarGo();break; 
     case COMM_UPL:   CarGoL();break; 
     case COMM_UPR:   CarGoR();break; 
     case COMM_LEFT:  CarLeft();break; 
     case COMM_RIGHT: CarRight();break; 
//     case COMM_STOP:  CarStop();break; 
     default : break; 
    } 
   } 
  } 
 } 
} 
 
void RCC_Conf(void) 
{ 
 ErrorStatus HSEStartUPStatus; 
 RCC_DeInit(); 
 RCC_HSEConfig(RCC_HSE_ON); 
 HSEStartUPStatus=RCC_WaitForHSEStartUp(); 
 if(HSEStartUPStatus == SUCCESS) 
 { 
  RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  RCC_PCLK2Config(RCC_HCLK_Div1); 
  RCC_PCLK1Config(RCC_HCLK_Div2); 
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9); 
  RCC_PLLCmd(ENABLE); 
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); 
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
  while(RCC_GetSYSCLKSource()!=0x08); 
 } 
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //RCC_APB1Periph_TIM2 
} 
 
void GPIO_Conf(void) 
{ 
 GPIO_InitTypeDef GPIO_InitStructure; 
  
 GPIO_InitStructure.GPIO_Pin=LED_PIN; 
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz; 
 GPIO_Init(GPIOC,&GPIO_InitStructure); 
} 
 
//红外光电对管初始化--寻迹使用 
void RedRayInit(void) 
{ 
 GPIO_InitTypeDef  GPIO_InitStructure; 
 //L2 
 GPIO_InitStructure.GPIO_Pin = SEARCH_L2_PIN;//配置使能GPIO管脚 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度 
 GPIO_Init(SEARCH_L2_GPIO , &GPIO_InitStructure);  
 //L1 
 GPIO_InitStructure.GPIO_Pin = SEARCH_L1_PIN;//配置使能GPIO管脚 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度 
 GPIO_Init(SEARCH_L1_GPIO , &GPIO_InitStructure);  
 //M 
 GPIO_InitStructure.GPIO_Pin = SEARCH_M_PIN;//配置使能GPIO管脚 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度 
 GPIO_Init(SEARCH_M_GPIO , &GPIO_InitStructure);  
 //R1 
 GPIO_InitStructure.GPIO_Pin = SEARCH_R1_PIN;//配置使能GPIO管脚 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度 
 GPIO_Init(SEARCH_R1_GPIO , &GPIO_InitStructure);  
 //R2 
 GPIO_InitStructure.GPIO_Pin = SEARCH_R2_PIN;//配置使能GPIO管脚 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度 
 GPIO_Init(SEARCH_R2_GPIO , &GPIO_InitStructure);  
 
} 
 
//打开103的启动文件starup_stm32f10x_md.s，这里面只有TIM1-TIM4的定时器中断事件，TIM6是没有中断响应这个功能的 
void TIM2_Init(void) 
{ 
    TIM_TimeBaseInitTypeDef  TIM2_TimeBaseStructure;   
    NVIC_InitTypeDef NVIC_InitStructure; 
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_Init(&NVIC_InitStructure); 
     
    TIM_TimeBaseStructInit(&TIM2_TimeBaseStructure); 
    TIM2_TimeBaseStructure.TIM_Prescaler = 72 - 1;  //分频之后的时钟频率为1MHz 
    TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM2_TimeBaseStructure.TIM_Period = 100 - 1;      //定时0.1ms 
    TIM2_TimeBaseStructure.TIM_ClockDivision = 0; 
    TIM_TimeBaseInit(TIM2, &TIM2_TimeBaseStructure); 
     
    TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Regular); 
     
    TIM_Cmd(TIM2, ENABLE); 
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
} 
   
void TIM2_IRQHandler(void) 
{ 
    if(TIM_GetITStatus(TIM2, TIM_IT_Update)== SET) //!=RESET 
    { 
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update); 
        tick_1ms++; 
  if(tick_1ms > 10) 
  { //1ms定时到 
   tick_1ms = 0; 
   speed_count++; 
   tick_5ms++; 
   if(speed_count > 50) 
   { 
    speed_count = 0; 
   } 
   CarMove(); 
 
  } 
 } 
} 
//五寻迹传感器 
void SEARCHPath(void) 
{ 
 uint8_t nowL2,nowL1,nowM,nowR1,nowR2,i; //now-当前测试状态 
 static uint8_t lastL2,lastL1,lastR1,lastR2; //last-前一测试状态 
 static uint8_t keepStateL=0,keepStateR=0,keepStateRS=0; //L(R)-正在左(右)转90标志,RS右转180标志 
  
 nowR2 = SEARCH_R2_IO; 
 nowR1 = SEARCH_R1_IO; 
 nowM  = SEARCH_M_IO; 
 nowL1 = SEARCH_L1_IO; 
 nowL2 = SEARCH_L2_IO; 
 
 if(keepStateL == 1) 
 { 
  if(nowL1==1 && lastL1==0) 
  { 
   keepStateL = 0; 
   noTurnRight=80; 
  } 
 } 
 else if(keepStateR == 1) 
 { 
  if(nowR1 == 1 && lastR1 == 0) //r2回出现转2圈的现象，改为r1 
  { 
   keepStateR = 0; 
  } 
 } 
 else if(keepStateRS == 1) 
 { 
  if(nowR1 == 1 && lastR1 == 0 && brCount == 0) 
  { 
   keepStateRS = 0; 
  } 
 } 
 else 
 { 
  if(nowR2 == 1) //右全黑 
  { 
   stopCountR++; 
 
   if(nowL2 == 1) //全黑 
    stopCountL++; 
  } 
  else if(nowL2==0 && lastL2==1) //当前白,前一刻黑,左转 
  { 
   //if(stopCountL == 0) //左转,此句防左T 
   if(nowM == 0 && stopCountL == 0) //左转,此句防左T 
   { 
    stopCountR = 0; 
    stopCountL = 0; 
    ctrl_comm = COMM_LEFT;//左转90 
    keepStateL = 1; 
   } 
  } 
  else if((nowL2==0 && nowR2==0) && (lastL2==0 && lastR2==0)) //全白 
  { 
   if(nowL1 == 1) //直行,偏左,需向右偏小微调 
   { 
    ctrl_comm = COMM_UPL; 
   } 
   else if(nowR1 == 1) //直行,偏右,需向左偏微调 
   { 
    ctrl_comm = COMM_UPR; 
   } 
   else if(nowM == 0) //死路 
   { 
    ctrl_comm = COMM_RIGHT;//右转180 
    keepStateR = 1; 
    for(i=0;i<255;i++); 
   } 
   else //直行路 
   { 
    ctrl_comm = COMM_UP; 
   } 
   stopCountR = 0; 
   stopCountL = 0; 
  } 
 
  if(stopCountR > 0 && stopCountR < 50) 
  { 
   if(nowR2 == 0 && noTurnRight == 0) //十,T,右转,右T 
   {GPIO_ResetBits(GPIOC,GPIO_Pin_13); 
    stopCountR = 0; 
    stopCountL = 0; 
 
    brCount = 50; 
    ctrl_comm = COMM_RIGHT;//右转90 
    keepStateRS = 1; 
   } 
  } 
 } 
  
 lastL2 = nowL2; 
 lastL1 = nowL1; 
 lastR1 = nowR1; 
 lastR2 = nowR2; 
} 

