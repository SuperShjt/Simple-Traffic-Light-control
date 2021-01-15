#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "tm4c123gh6pm.h"
#include "GPTM.h"


int main()
{
  
  
  PORTF_init();
  PORTB_init();
  PORTC_init();
  PORTD_init();
  GPTM1_init();
  GPTM2_init();
  GPTM3_init();

  while(1)
  {
    //GPIO_PORTF_DATA_R=0x04;
    __asm("    wfi\n");
  }
  
  return 0;
}
