#ifndef __PWM_OUTPUT_H
#define __PWM_OUTPUT_H

#include "io.h"				// including stdint.h stdbool.h
#include "timer.h"
#include "ledTimer.h"
#include "motors.h"			// including mixer.h

typedef enum {
    PWM_TYPE_STANDARD = 0,
    PWM_TYPE_ONESHOT125,
    PWM_TYPE_ONESHOT42,
    PWM_TYPE_MULTISHOT,
    PWM_TYPE_BRUSHED,
#ifdef USE_DSHOT
    PWM_TYPE_DSHOT150,
    PWM_TYPE_DSHOT300,
    PWM_TYPE_DSHOT600,
    PWM_TYPE_DSHOT1200,
#endif
    PWM_TYPE_MAX
} motorPwmProtocolTypes_e;

#define PWM_TIMER_KHZ         10
#define PWM_TIMER_MHZ         1

typedef struct {
    volatile timCCR_t *ccr;
    TIM_TypeDef *tim;
    bool forceOverflow;
    uint16_t period;
    bool enabled;
    IO_t io;
} pwmOutputPort_t;

typedef void (*pwmWriteFuncPtr)(uint8_t index, uint16_t value);			// function pointer used to write motors

void motorInit(const motorConfig_t *motorConfig, uint16_t idlePulse, uint8_t motorCount);
void ledTimerInit(const LedTimerConfig_t *ledTimerConfig, uint16_t idlePulse, uint8_t ledCount, uint32_t KhzGain);
void pwmWriteLed(uint8_t index, uint16_t value);

#endif	// __PWM_OUTPUT_H
