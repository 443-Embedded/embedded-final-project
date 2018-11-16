#ifndef SYSTEM_STRUCTURES_H
#define SYSTEM_STRUCTURES_H

#define PCON_ADDRESS	0x400FC0C0
#define PCON	*((volatile uint32_t*)(PCON_ADDRESS))

#define PCONP_ADDRESS	0x400FC0C4
#define PCONP	*((volatile uint32_t*)(PCONP_ADDRESS))

#define PERIPHERAL_CLOCK_FREQUENCY 60000000

#endif
