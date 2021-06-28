#include <stdio.h>
#include "MKL16Z4.h"

volatile uint32_t g_msTicks = 0;


void _delay_ms(uint32_t del)
{
  uint32_t curTicks;
  curTicks = g_msTicks;
   while ((g_msTicks - curTicks) < del)
  {
    __asm("wfi");
  }
}


void delay_us(int a1)
{
  int count = 768 * a1;
  while (count)
  {
    count--;
  }
}

