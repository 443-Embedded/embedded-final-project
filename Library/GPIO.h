#ifndef GPIO_H
#define GPIO_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"
#include "Parameters.h"

//Correctly define GPIO_TypeDef struct.
typedef struct
{
  volatile	uint32_t DIR;
						uint32_t RESERVED0[3];
  volatile	uint32_t MASK;
  volatile	uint32_t PIN;
  volatile	uint32_t SET;
  volatile  uint32_t CLR;
} GPIO_TypeDef;

typedef enum {
	INPUT = 0,
	OUTPUT = 1
} GPIO_Direction;

typedef enum {
	LOW = 0,
	HIGH = 1
} GPIO_Value;


#define GPIO_ADDRESS	0x20098000

#define PORT0	((GPIO_TypeDef*) PORT0_BASE)
#define PORT1	((GPIO_TypeDef*) PORT1_BASE)
#define PORT2	((GPIO_TypeDef*) PORT2_BASE)
#define PORT3	((GPIO_TypeDef*) PORT3_BASE)
#define PORT4	((GPIO_TypeDef*) PORT4_BASE)
#define PORT5	((GPIO_TypeDef*) PORT5_BASE)

#define PORT0_BASE		(GPIO_ADDRESS + 0x000)
#define PORT1_BASE		(GPIO_ADDRESS + 0x020)
#define PORT2_BASE		(GPIO_ADDRESS + 0x040)
#define PORT3_BASE		(GPIO_ADDRESS + 0x060)
#define PORT4_BASE		(GPIO_ADDRESS + 0x080)
#define PORT5_BASE		(GPIO_ADDRESS + 0x0A0)

#define IOCON_LEFT_ADDR	0x4002C280
#define IOCON_LEFT	*((volatile uint32_t*)(IOCON_LEFT_ADDR))

#define IOCON_DOWN_ADDR	0x4002C284
#define IOCON_DOWN	*((volatile uint32_t*)(IOCON_DOWN_ADDR))

#define IOCON_UP_ADDR	0x4002C288
#define IOCON_UP	*((volatile uint32_t*)(IOCON_UP_ADDR))
	
#define IOCON_CENTER_ADDR	0x4002C28C
#define IOCON_CENTER	*((volatile uint32_t*)(IOCON_CENTER_ADDR))
	
#define IOCON_RIGHT_ADDR 0x4002C290
#define IOCON_RIGHT	*((volatile uint32_t*)(IOCON_RIGHT_ADDR))

void GPIO_Init(void);
uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK);

#endif
