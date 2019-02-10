#include "stm32f10x.h" 
#include "motor.h" 
#include "interface.h" 
#include "string.h" 
 
//ȫ�ֱ������� 
uint16_t speed_count=0;//ռ�ձȼ����� 50��һ���� 
int8_t left_speed_duty=MIN_SPEED_DUTY; 
int8_t right_speed_duty=MIN_SPEED_DUTY; 
 
uint8_t tick_5ms = 0;//5ms����������Ϊ�������Ļ������� 
uint8_t tick_1ms = 0;//1ms����������Ϊ����Ļ��������� 
 
int8_t ctrl_comm = COMM_STOP;//����ָ�� 
int8_t ctrl_comm_last = COMM_STOP;//��һ�ε�ָ�� 
uint8_t continue_time=0; 
 
uint16_t stopCountR=0,stopCountL=0,brCount=0,noTurnRight=0; 
//no -�˲���=0ǰ,��ֹ��ת.�޴˲���,��ת����,������������ת״̬. 
//br -��ת90ʱ,�޴˲���������ֹͣת��. 
//stopL(R) -��(��)������ߵ�ʱ��,�̱�ʾ��ʮ�ֻ�T,�����յ� 
 
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
   if(ctrl_comm_last != ctrl_comm)//ָ����仯 
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
 
//������Թܳ�ʼ��--Ѱ��ʹ�� 
void RedRayInit(void) 
{ 
 GPIO_InitTypeDef  GPIO_InitStructure; 
 //L2 
 GPIO_InitStructure.GPIO_Pin = SEARCH_L2_PIN;//����ʹ��GPIO�ܽ� 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,�������� 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ� 
 GPIO_Init(SEARCH_L2_GPIO , &GPIO_InitStructure);  
 //L1 
 GPIO_InitStructure.GPIO_Pin = SEARCH_L1_PIN;//����ʹ��GPIO�ܽ� 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,�������� 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ� 
 GPIO_Init(SEARCH_L1_GPIO , &GPIO_InitStructure);  
 //M 
 GPIO_InitStructure.GPIO_Pin = SEARCH_M_PIN;//����ʹ��GPIO�ܽ� 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,�������� 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ� 
 GPIO_Init(SEARCH_M_GPIO , &GPIO_InitStructure);  
 //R1 
 GPIO_InitStructure.GPIO_Pin = SEARCH_R1_PIN;//����ʹ��GPIO�ܽ� 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,�������� 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ� 
 GPIO_Init(SEARCH_R1_GPIO , &GPIO_InitStructure);  
 //R2 
 GPIO_InitStructure.GPIO_Pin = SEARCH_R2_PIN;//����ʹ��GPIO�ܽ� 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,�������� 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ� 
 GPIO_Init(SEARCH_R2_GPIO , &GPIO_InitStructure);  
 
} 
 
//��103�������ļ�starup_stm32f10x_md.s��������ֻ��TIM1-TIM4�Ķ�ʱ���ж��¼���TIM6��û���ж���Ӧ������ܵ� 
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
    TIM2_TimeBaseStructure.TIM_Prescaler = 72 - 1;  //��Ƶ֮���ʱ��Ƶ��Ϊ1MHz 
    TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM2_TimeBaseStructure.TIM_Period = 100 - 1;      //��ʱ0.1ms 
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
  { //1ms��ʱ�� 
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
//��Ѱ�������� 
void SEARCHPath(void) 
{ 
 uint8_t nowL2,nowL1,nowM,nowR1,nowR2,i; //now-��ǰ����״̬ 
 static uint8_t lastL2,lastL1,lastR1,lastR2; //last-ǰһ����״̬ 
 static uint8_t keepStateL=0,keepStateR=0,keepStateRS=0; //L(R)-������(��)ת90��־,RS��ת180��־ 
  
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
  if(nowR1 == 1 && lastR1 == 0) //r2�س���ת2Ȧ�����󣬸�Ϊr1 
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
  if(nowR2 == 1) //��ȫ�� 
  { 
   stopCountR++; 
 
   if(nowL2 == 1) //ȫ�� 
    stopCountL++; 
  } 
  else if(nowL2==0 && lastL2==1) //��ǰ��,ǰһ�̺�,��ת 
  { 
   //if(stopCountL == 0) //��ת,�˾����T 
   if(nowM == 0 && stopCountL == 0) //��ת,�˾����T 
   { 
    stopCountR = 0; 
    stopCountL = 0; 
    ctrl_comm = COMM_LEFT;//��ת90 
    keepStateL = 1; 
   } 
  } 
  else if((nowL2==0 && nowR2==0) && (lastL2==0 && lastR2==0)) //ȫ�� 
  { 
   if(nowL1 == 1) //ֱ��,ƫ��,������ƫС΢�� 
   { 
    ctrl_comm = COMM_UPL; 
   } 
   else if(nowR1 == 1) //ֱ��,ƫ��,������ƫ΢�� 
   { 
    ctrl_comm = COMM_UPR; 
   } 
   else if(nowM == 0) //��· 
   { 
    ctrl_comm = COMM_RIGHT;//��ת180 
    keepStateR = 1; 
    for(i=0;i<255;i++); 
   } 
   else //ֱ��· 
   { 
    ctrl_comm = COMM_UP; 
   } 
   stopCountR = 0; 
   stopCountL = 0; 
  } 
 
  if(stopCountR > 0 && stopCountR < 50) 
  { 
   if(nowR2 == 0 && noTurnRight == 0) //ʮ,T,��ת,��T 
   {GPIO_ResetBits(GPIOC,GPIO_Pin_13); 
    stopCountR = 0; 
    stopCountL = 0; 
 
    brCount = 50; 
    ctrl_comm = COMM_RIGHT;//��ת90 
    keepStateRS = 1; 
   } 
  } 
 } 
  
 lastL2 = nowL2; 
 lastL1 = nowL1; 
 lastR1 = nowR1; 
 lastR2 = nowR2; 
} 

