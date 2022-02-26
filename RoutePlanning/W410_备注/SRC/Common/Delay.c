#include "Include.h"

volatile uint32_t g_delay_counter = 0;

void delay(uint16_t time)
{
  g_delay_counter=0;
  while(g_delay_counter<time);
}

void delay_increase_counter(void)
{
  g_delay_counter++;
}













