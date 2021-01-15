#include <stdint.h>
#include <stdbool.h>
#include "GPTM.h"
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

static int state=0;
bool flag = true;

void GPTM1_init()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
  TimerDisable(TIMER0_BASE,TIMER_BOTH);
  TimerLoadSet(TIMER0_BASE,TIMER_BOTH,16000000-1);
  TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC_UP);
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  TimerControlStall(TIMER0_BASE,TIMER_BOTH,true);
  IntPrioritySet(INT_TIMER0A,0x20);
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER0_BASE, TIMER_A,carshandler);
  TimerEnable(TIMER0_BASE,TIMER_BOTH);
}

void GPTM2_init()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));
  TimerDisable(TIMER1_BASE,TIMER_BOTH);
  TimerLoadSet(TIMER1_BASE,TIMER_BOTH,32000000-1);
  TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC_UP);
  TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
  TimerControlStall(TIMER1_BASE,TIMER_BOTH,true);
  IntPrioritySet(INT_TIMER1A,0x20);
  TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER1_BASE, TIMER_A,Timer2Handler);
  
}
void GPTM3_init()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2));
  TimerDisable(TIMER2_BASE,TIMER_BOTH);
  TimerLoadSet(TIMER2_BASE,TIMER_BOTH,32000000-1);
  TimerConfigure(TIMER2_BASE,TIMER_CFG_PERIODIC_UP);
  TimerIntClear(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
  TimerControlStall(TIMER2_BASE,TIMER_BOTH,true);
  IntPrioritySet(INT_TIMER2A,0x20);
  TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER2_BASE, TIMER_A,Timer3Handler);
  
}
/*
void GPTM4_init()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER3));
  TimerDisable(TIMER3_BASE,TIMER_BOTH);
  TimerLoadSet(TIMER3_BASE,TIMER_BOTH,16000000-1);
  TimerConfigure(TIMER3_BASE,TIMER_CFG_PERIODIC_UP);
  TimerIntClear(TIMER3_BASE,TIMER_TIMA_TIMEOUT);
  TimerControlStall(TIMER3_BASE,TIMER_BOTH,true);
  IntPrioritySet(INT_TIMER3A,0x20);
  TimerIntEnable(TIMER3_BASE,TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER3_BASE, TIMER_A,flag_Handler);
  TimerEnable(TIMER3_BASE,TIMER_BOTH);
}
*/
void PORTF_init()
{
  clock|=0x20;
  DirF=0x0E;
  DenF=0x0E;
}
void PORTB_init()
{
  clock |=0x02;
  GPIO_PORTB_DIR_R=0x0E;
  GPIO_PORTB_DEN_R=0x0E;
}
void PORTC_init()
{
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//Enable Clock for Port C
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)){}//Check
      GPIOIntRegister(GPIO_PORTC_BASE, Pedestrian1);//Interput Handler
      GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_4);
      GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
      GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
      GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
      GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);
}
void PORTD_init()
{
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//Enable Clock for Port C
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}//Check
      GPIOIntRegister(GPIO_PORTD_BASE, Pedestrian2);//Interput Handler
      GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_2);
      GPIOPadConfigSet(GPIO_PORTD_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
      GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_6);
      GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_FALLING_EDGE);
      GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_2);
}
void carshandler(void){
  TIMER0_ICR_R= TIMER_ICR_TATOCINT;
  flag=true;
  switch(state){
  case 0:
    GPIO_PORTF_DATA_R=0x02; //GREEN CARS 1
    GPIO_PORTB_DATA_R=0x08; //RED CARS2
    GPIO_PORTD_DATA_R =0x40;//Pedestrian1
    GPIO_PORTC_DATA_R =0x40; //Pedestrian2
     state++;
    break;
  case 5:
    GPIO_PORTF_DATA_R=0x04; //YELLOW CARS1
    //GPIO_PORTB_DATA_R=0x08;
     state++;
     
    break;
  case 7:
    GPIO_PORTF_DATA_R=0x08; //RED CARS 1
    GPIO_PORTB_DATA_R=0x08; //RED CARS 2
     state++;
    break;
  case 8:
    //GPIO_PORTF_DATA_R=0x02;
    GPIO_PORTB_DATA_R=0x02; //GREEN CARS2
     state++;
    break;
  case 13:
    //GPIO_PORTF_DATA_R=0x02;
    GPIO_PORTB_DATA_R=0x04; //YELLOW CARS2
     state++;
    break;
  case 15:
    GPIO_PORTF_DATA_R=0x08; //RED CARS1
    GPIO_PORTB_DATA_R=0x08; //RED CARS2
     state++;
    break;
  case 16:
    GPIO_PORTF_DATA_R=0x02; //GREEN CARS1
    GPIO_PORTB_DATA_R=0x08; //RED CARS2
    state=0;
     state++;
    break;
  default:
    state++;
  }
  //state++;
}
void Pedestrian1(void)
{
  GPIOIntClear(GPIO_PORTC_BASE,GPIO_PIN_4);
  if ( GPIO_PORTC_DATA_R ==0x20){
    return;
  }
  else if (!flag){return;}
  
  GPIO_PORTC_DATA_R =0x20; //Pedestrian1
   TimerDisable(TIMER0_BASE,TIMER_BOTH);
    GPIO_PORTF_DATA_R=0x08; //RED CARS1
    GPIO_PORTB_DATA_R=0x08; //RED CARS2
   TimerEnable(TIMER1_BASE,TIMER_BOTH);
   
}
void Pedestrian2(void)
{
  GPIOIntClear(GPIO_PORTD_BASE,GPIO_PIN_2);
  
  if ( GPIO_PORTD_DATA_R ==0x20){
    return;
  }
   else if (!flag){return;}
  
  
  GPIO_PORTD_DATA_R =0x08; //Pedestrian2
  TimerDisable(TIMER0_BASE,TIMER_BOTH);
    GPIO_PORTF_DATA_R=0x08; //RED CARS1
    GPIO_PORTB_DATA_R=0x08; //RED CARS2
   TimerEnable(TIMER2_BASE,TIMER_BOTH);
  
}
void Timer2Handler(void)
{
  TIMER1_ICR_R= TIMER_ICR_TATOCINT;
  GPIO_PORTC_DATA_R =0x40; //RED Pedestrian1
  TimerDisable(TIMER1_BASE,TIMER_BOTH);
  TimerLoadGet(TIMER0_BASE,TIMER_BOTH);
  TimerEnable(TIMER0_BASE,TIMER_BOTH);
  flag = false;
  TimerLoadGet(TIMER0_BASE,TIMER_BOTH);
  
}
void Timer3Handler(void)
{
  TIMER2_ICR_R= TIMER_ICR_TATOCINT;
  GPIO_PORTD_DATA_R =0x40; //RED Pedestrian2
  TimerDisable(TIMER2_BASE,TIMER_BOTH);
  TimerLoadGet(TIMER0_BASE,TIMER_BOTH);
  TimerEnable(TIMER0_BASE,TIMER_BOTH);
  flag = false;
  
}

