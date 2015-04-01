/***************************************************************/
#include "stm32f10x.h"
#include "tim.h"
/***************************************************************/

void TIM_Configuration(TIM_TypeDef* TIMER,u16 Period,u16 Prescaler,u8 PP)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(TIMER==TIM1)     RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(TIMER==TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(TIMER==TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(TIMER==TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	else if(TIMER==TIM5)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	else if(TIMER==TIM6)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	else if(TIMER==TIM7)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	else if(TIMER==TIM8)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = Period-1; 	              //这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
	TIM_TimeBaseStructure.TIM_Prescaler =Prescaler-1;             // 这个就是预分频系数，当由于为0时表示不分频所以要减1  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	              // 使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInit(TIMER, &TIM_TimeBaseStructure);              //初始化定时器

	TIM_ClearITPendingBit(TIMER, TIM_IT_Update);             	  //Clear TIM5 update pending flag[清除TIM5溢出中断标志] 
	TIM_ITConfig(TIMER, TIM_IT_Update, ENABLE);				      //打开溢出中断
	TIM_Cmd(TIMER, ENABLE);                                       //计数器使能，开始工作
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	if(TIMER==TIM2){NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;}
	else if(TIMER==TIM3){NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;}
	else if(TIMER==TIM4){NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;}
	else if(TIMER==TIM5){NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;}
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =PP;	 //主优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 //从优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能中断通道
	NVIC_Init(&NVIC_InitStructure);								 //用以上参数初始化10-15号线中断通道
}


////定时器5通道1输入捕获配置

//TIM_ICInitTypeDef  TIM2_ICInitStructure;

//void TIM2_Cap_Init(u16 arr,u16 psc)
//{	 
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM5时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
//	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 下拉
//	
//	//初始化定时器2 TIM2	 
//	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//  
//	//初始化TIM5输入捕获参数
//	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
//	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
//	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
//	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
//	
//	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;  //先占优先级2级
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
//	
//	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
//  TIM_Cmd(TIM2,ENABLE ); 	//使能定时器5
//}
