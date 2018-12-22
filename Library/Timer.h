#ifndef TIMER_H
#define TIMER_H

#include "LPC407x_8x_177x_8x.h"
#include "SystemStructures.h"
#include "Parameters.h"
#include "LED.h"
#include "main.h"

typedef struct {
  volatile	uint32_t IR;
  volatile	uint32_t TCR;
  volatile	uint32_t TC;
  volatile	uint32_t PR;
  volatile	uint32_t PC;
  volatile	uint32_t MCR;
  volatile	uint32_t MR0;
  volatile	uint32_t MR1;
  volatile	uint32_t MR2;
  volatile	uint32_t MR3;
  volatile	uint32_t CCR;
  volatile	uint32_t CR0;
  volatile	uint32_t CR1;
						uint32_t RESERVED0[2];
  volatile	uint32_t EMR;
						uint32_t RESERVED1[12];
  volatile	uint32_t CTCR;
} TIMER_TypeDef;

#define TIMER0_BASE	0x40004000
#define TIMER1_BASE	0x40008000
#define TIMER2_BASE	0x40090000
#define TIMER3_BASE	0x40094000

#define TIMER0	((TIMER_TypeDef*) TIMER0_BASE)
#define TIMER1	((TIMER_TypeDef*) TIMER1_BASE)
#define TIMER2	((TIMER_TypeDef*) TIMER2_BASE)
#define TIMER3	((TIMER_TypeDef*) TIMER3_BASE)

#define IOCON_TIMER2_CAP1_ADDRESS 0x4002C014
#define IOCON_TIMER2_CAP1 *((volatile uint32_t*)(IOCON_TIMER2_CAP1_ADDRESS))

#define IOCON_TIMER2_CAP0_ADDRESS 0x4002C010
#define IOCON_TIMER2_CAP0 *((volatile uint32_t*)(IOCON_TIMER2_CAP0_ADDRESS))

void Timer1_Init(void);
void Timer2_Init(void);
void Timer3_Init(void);

void TIMER1_Start(void);
void TIMER1_Stop(void);

void TIMER3_Start(void);
void TIMER3_Stop(void);

void TIMER2_Start(void);
void TIMER2_Stop(void);

#endif
