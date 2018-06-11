#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#define  baud_rate_L SCI0BDL 
#define  baud_rate_H SCI0BDH 
#define  loop_back_en  SCI0CR1_LOOPS
#define  rx_int0_ext1 SCI0CR1_RSRC
#define  mode SCI0CR1_M
#define  parity_bit SCI0CR1_PE

#define  tx_interrupt_en SCI0CR2_TIE
#define  rx_interrupt_en SCI0CR2_RIE
#define  tx_en SCI0CR2_TE 
#define  rx_en SCI0CR2_RE

#define  pwm_clk_en PWMCLK
#define  pwm_A0_B1  PWMCLKAB
#define  pwm_prescal_A PWMPRCLK
 
#define  polarity PWMPOL
#define  left0_center1_alignment  PWMCAE
#define  periodo_ch0 PWMPER0
#define  duty_ch0 PWMDTY0

#define  rx_reg_full_flag SCI0SR1_RDRF
#define  tx_reg_empty_flag SCI0SR1_TDRE



         
#define  phase_shift 64 

#define risoluzione ATDCTL1_SRES0
#define giustificazione ATDCTL3_DJM
#define scan_loop ATDCTL5_SCAN
#define pot_interr_en ATDCTL2_ASCIE
#define pot_conv ATDDR0

#define pot_conv_flag ATDSTAT2L_CCF0

int tx_val;
int rx_val;


void main(void) {
  /* put your own code here */
  
  //SCIO setup
  baud_rate_L=41;
  baud_rate_H=0;
  loop_back_en=1;
  rx_int0_ext1=0;
  mode=0;
  parity_bit=0;
  
  tx_interrupt_en=1;  //interrut tx vuoto, pronto a prendersi il dato da trasmettere
  rx_interrupt_en=1;  //rx pieno, ha ricevuto un nuovo dato
  tx_en=1;         //tx abilitato
  rx_en=1;         //rx abilitato
  
  
 
  //ATD potenziometro setup
  risoluzione=0;
  pot_interr_en=1;
  giustificazione=1;
  scan_loop=1;
  
  // PWM  - setup e configuration
	pwm_clk_en=0x00;  //scelgo clka del canale  0
	pwm_A0_B1=0x00; //scelgo il clka per il  canali 0
	pwm_prescal_A=0x02; //setto il prescaler di clka per avere T=640 ns 
	polarity=1; //scelgo la polarità dll'onda
	left0_center1_alignment=0; //scelgo il left alignment
	
	periodo_ch0=255;      // setto il periodo iniziale a 1
	duty_ch0=127;      // setto il duty cycle iniziale al 50%      
	 
  PWMCNT0=0;             //clear il counter;
  PWME_PWME0=1;         //abilito il registro
  
  
 
  
  
   


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
#pragma CODE_SEG NON_BANKED

#pragma CODE_SEG DEFAULT
