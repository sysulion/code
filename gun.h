#ifndef __GUN_H__
#define __GUN_H__
#include "main.h"
#ifndef FRICTION_WHEEL
#define FRICTION_WHEEL
#endif 

#if defined(FRICTION_WHEEL)

#define PWM1  TIM5->CCR1
#define PWM2  TIM5->CCR2
#define PWM3  TIM9->CCR1

#define InitFrictionWheel()     \
        PWM1 = 500;             \
        PWM2 = 500;
#define SetFrictionWheelSpeed(x) \
        PWM1 = x;                \
        PWM2 = x;

#endif 
#define MotorAngle 1011

typedef enum {Running,Stop,KeepRunning,FirstRunning,FirstKeepRunning}ProjectilaMotor;
void PWM_Configuration(void);
void SetMotorSpeed(uint16_t uTimeCnt); 
void ProjectileMotor_Config(void);
void ShootOneProjectile(void);
void Shoot(void);
void StopShooting(void);
void Shoot_Configuration(void);
#endif /* __GUN_H__*/
