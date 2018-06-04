#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define  pwm_clk_en PWMCLK
#define  pwm_A0_B1  PWMCLKAB
#define  pwm_prescal_A PWMPRCLK
 
#define  polarity PWMPOL
#define  left0_center1_alignment  PWMCAE
#define  pwm_per_ch0 PWMPER0
#define  pwm_per_ch1 PWMPER1
#define  pwm_per_ch2 PWMPER2
#define  pwm_per_ch3 PWMPER3
#define  pwm_dty_ch0 PWMDTY0
#define  pwm_dty_ch1 PWMDTY1
#define  pwm_dty_ch2 PWMDTY2
#define  pwm_dty_ch3 PWMDTY3

         
#define  phase_shift 64 

#define risoluzione ATDCTL1_SRES0
#define giustificazione ATDCTL3_DJM
#define scan_loop ATDCTL5_SCAN
#define interr_en ATDCTL2_ASCIE
#define pot_conv ATDDR0

#define pot_conv_flag ATDSTAT2L_CCF0

void main(void) {
  /* put your own code here */
 
 	// PWM part A - setup e configuration
	pwm_clk_en=0x00;  //scelgo clka dei canali 0,1,2,3
	pwm_A0_B1=0x0C; //scelgo il clka per i  canali 0,1,2,3
	pwm_prescal_A=0x03; //setto il prescaler di clka
	polarity=0x09;  //imposto la polarity di ogni canale
	left0_center1_alignment=0x03; //scelgo center_alignment
	
	pwm_per_ch0=0x7F;      // setto il periodo di ch0 a 127 (center alignment x2)
	pwm_per_ch1=0x7F;
	pwm_per_ch2=0xFF;      //setto il per di ch2 a 255 (left alignment)
	pwm_per_ch3=0xFF;
	
	pwm_dty_ch0=0x7F;      // setto il duty cycle
	pwm_dty_ch1=0x7F;      
	pwm_dty_ch2=0x7F;      
	pwm_dty_ch3=0x7F;      
	 
  PWMCNT0=0;
  PWMCNT1=0;
  PWMCNT2=0;
  PWMCNT3=0;
  
  PWME_PWME0=1;         //abilito il registro
  
  PWME_PWME1=1;
  PWME_PWME2=1;
  PWME_PWME3=1;
  
  
  //abilito l'interrupt generato dal potenziometro
  risoluzione=0;
  interr_en=1;
  giustificazione=1;
  scan_loop=1;
  
  
 
  
  
   


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
#pragma CODE_SEG NON_BANKED

interrupt 22 void adc_interrupt(void) {
   interr_en=0;
   
if( pot_conv_flag ) {
 
  pwm_per_ch0=0x7F-ATDDR0/2; 
  pwm_per_ch1=0x7F-ATDDR0/2;
  pwm_per_ch2=0xFF-ATDDR0;
  pwm_per_ch3=0xFF-ATDDR0;
  
}

}

#pragma CODE_SEG DEFAULT
