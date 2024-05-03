#ifndef _PWM_H_
#define _PWM_H_

#include <stdint.h>

#define Servo_init() TIM_PWM_Init(20000 - 1, 72 - 1)

#define TIM2PWM(C, T) TIM2->CCR##C = (uint16_t)(T)
#define TIM3PWM(C, T) TIM3->CCR##C = (uint16_t)(T)
#define TIM4PWM(C, T) TIM4->CCR##C = (uint16_t)(T)

#define PA0_PWM(T)		TIM2PWM(1,T)
#define PA1_PWM(T)		TIM2PWM(2,T)
#define PA2_PWM(T)		TIM2PWM(3,T)
#define PA3_PWM(T)		TIM2PWM(4,T)
#define PA6_PWM(T)		TIM3PWM(1,T)
#define PA7_PWM(T)		TIM3PWM(2,T)
#define PA15_PWM(T)		TIM2PWM(1,T)
#define PB0_PWM(T)		TIM3PWM(3,T)
#define PB1_PWM(T)		TIM3PWM(4,T)
#define PB3_PWM(T)		TIM2PWM(2,T)
#define PB4_PWM(T)		TIM3PWM(1,T)
#define PB5_PWM(T)		TIM3PWM(2,T)
#define PB6_PWM(T)		TIM4PWM(1,T)
#define PB7_PWM(T)		TIM4PWM(2,T)
#define PB8_PWM(T)		TIM4PWM(3,T)
#define PB9_PWM(T)		TIM4PWM(4,T)
#define PB10_PWM(T)		TIM2PWM(3,T)
#define PB11_PWM(T)		TIM2PWM(4,T)

#define Servo_PA0(W)	TIM2PWM(1,500 + (W) / 0.09)
#define Servo_PA1(W)	TIM2PWM(2,500 + (W) / 0.09)
#define Servo_PA2(W)	TIM2PWM(3,500 + (W) / 0.09)
#define Servo_PA3(W)	TIM2PWM(4,500 + (W) / 0.09)
#define Servo_PA6(W)	TIM3PWM(1,500 + (W) / 0.09)
#define Servo_PA7(W)	TIM3PWM(2,500 + (W) / 0.09)
#define Servo_PA15(W)	TIM2PWM(1,500 + (W) / 0.09)
#define Servo_PB0(W)	TIM3PWM(3,500 + (W) / 0.09)
#define Servo_PB1(W)	TIM3PWM(4,500 + (W) / 0.09)
#define Servo_PB3(W)	TIM2PWM(2,500 + (W) / 0.09)
#define Servo_PB4(W)	TIM3PWM(1,500 + (W) / 0.09)
#define Servo_PB5(W)	TIM3PWM(2,500 + (W) / 0.09)
#define Servo_PB6(W)	TIM4PWM(1,500 + (W) / 0.09)
#define Servo_PB7(W)	TIM4PWM(2,500 + (W) / 0.09)
#define Servo_PB8(W)	TIM4PWM(3,500 + (W) / 0.09)
#define Servo_PB9(W)	TIM4PWM(4,500 + (W) / 0.09)
#define Servo_PB10(W)	TIM2PWM(3,500 + (W) / 0.09)
#define Servo_PB11(W)	TIM2PWM(4,500 + (W) / 0.09)

void TIM_PWM_Init(uint16_t arr,uint16_t psc);

#endif
