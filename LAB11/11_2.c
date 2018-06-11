#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#define LED1 PTT_PTT4
#define LED2 PTT_PTT5
#define LED3 PTT_PTT6
#define LED4 PTT_PTT7

#define output_compare_en TIOS
#define prescale TSCR2_PR
#define arm_ch_0 TIE_C0I
#define arm_ch_1 TIE_C1I
#define arm_ch_2 TIE_C2I
#define arm_ch_3 TIE_C3I
#define time_count_reg_en TSCR1_TEN

#define ch0_flag  TFLG1_C0F
#define ch1_flag  TFLG1_C1F
#define ch2_flag  TFLG1_C2F
#define ch3_flag  TFLG1_C3F
  
#define time_counter TCNT

#define cnt0_stored TC0
#define cnt1_stored TC1
#define cnt2_stored TC2
#define cnt3_stored TC3

#define delta 24414

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

#define risoluzione ATDCTL1_SRES0
#define giustificazione ATDCTL3_DJM
#define scan_loop ATDCTL5_SCAN
#define pot_interr_en ATDCTL2_ASCIE
#define pot_conv ATDDR0

#define pot_conv_flag ATDSTAT2L_CCF0

int rx_val;
int timO0;
int timO1;
int timO2;
int timO3;
char atd_val;


void main(void) {
  /* put your own code here */
   //led setup
   
    DDRT= 0xFF; //abilito i led come output e lascio come input l'ic
    LED1=1; //spengo tutti i led @inizio
    LED2=1;
    LED3=1;
    LED4=1;
    
    output_compare_en=1;
    prescale=7; //set il prescale a 7 to have 48,828 KHz
    
    arm_ch_0=1; //attivo la generazione di interrupt
    arm_ch_1=1; 
    arm_ch_2=1; 
    arm_ch_3=1;
     
    time_count_reg_en=1; //abilito il time counter register
    
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
	pwm_prescal_A=3; //setto il prescaler di clka per avere f=6.15 KHz
	polarity=1; //scelgo la polarità dll'onda
	left0_center1_alignment=0; //scelgo il left alignment
	
	periodo_ch0=255;      // setto il periodo iniziale a 1
	duty_ch0=127;      // setto il duty cycle iniziale al 50%      
	 
  PWMCNT0=0;             //clear il counter;
  PWME_PWME0=1;         //abilito il registro 
  
  cnt0_stored=TCNT+delta/2;
  cnt1_stored=TCNT+delta/2;
  cnt2_stored=TCNT+delta/2;
  cnt3_stored=TCNT+delta/2;
  
  
 	EnableInterrupts;
  
  for(;;) {
  
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

#pragma CODE_SEG NON_BANKED

interrupt 8 void TIM_int0(void)  // 8=(255-238)/2 per difetto 
{
    if(ch0_flag){
     LED1^=1; 
     cnt0_stored+=timO0;
     ch1_flag=0;
    }
}
interrupt 9 void TIM_int1(void) {
    if(ch1_flag){
     LED2^=1; 
    cnt1_stored+=timO1;
     ch1_flag=0;
    }
}
interrupt 10 void TIM_int2(void) {
    if(ch2_flag){
     LED3^=1; 
    cnt2_stored+=timO2;
    ch2_flag=0;
    }
}
interrupt 11 void TIM_int3(void) {
    if(ch3_flag){
     LED4^=1; 
    cnt3_stored+=timO3;
    ch3_flag=0;
    }

}


interrupt 22 void atd_interrupt(void) {

   
if( pot_conv_flag ) {
 
 atd_val=ATDDR0;
 
 }
 pot_conv_flag=0;
 
 //leggo il val ricevuto e aggiorno periodo e duty  
 periodo_ch0=rx_val;
 duty_ch0=rx_val/2;

}
interrupt 20 void sci_interrupt(void){

if(rx_reg_full_flag){
  
  rx_val=SCI0DRL;

if(rx_val=='0') {
  timO0=delta/2+atd_val;
} 
if(rx_val=='1') {
  timO1=delta/2+atd_val;
} 
if(rx_val=='2') {
  timO2=delta/2+atd_val;
} 
if(rx_val=='3') {
  timO3=delta/2+atd_val;
} 

if(rx_val=='d') {
  duty_ch0=atd_val/2;
} 
if(rx_val=='t') {
  periodo_ch0=atd_val;
} 


 }
}
#pragma CODE_SEG DEFAULT