#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define signal_wave PTT_PTT0
#define input_capture_en TIOS
#define prescale TSCR2_PR
#define arm_channel_0 TIE_C0I
#define time_count_reg_en TSCR1_TEN
void TIM_interrupt_init1(void) 
{
  prescale=1; //attivo il prescale
  input_capture_en=0x00; //@RESET, quindi è già configurato come input capture  
  arm_channel_0=1; //attivo la generazione di interrupt
  time_count_reg_en=1;
  
  
}

void main(void) {
  /* put your own code here */
  
  //TSCR1=0x90; //attivo contatore e fast flag clear
  TIM_interrupt_init1();


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
