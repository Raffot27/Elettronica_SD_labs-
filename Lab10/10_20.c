#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define signal_wave PTT_PTT0   //l'input dell' onda che ricevo

#define input_capture_en TIOS
#define prescale TSCR2_PR
#define arm_ch_0 TIE_C0I
#define time_count_reg_en TSCR1_TEN
#define flag_ch_0  TFLG1_C0F
#define time_counter TCNT
#define time_counter_stored TC0
                  
#define  confA TCTL4_EDG0A  //mi configurano a cosa sarà sensibile il capture 
#define  confB TCTL4_EDG0B

#define  pwm_clk_en_ch0 PWMCLK_PCLK0
#define  pwm_A0_B1_ch0  PWMCLKAB_PCLKAB0
#define  pwm_prescal_A PWMPRCLK_PCKA 
#define  polarity PWMPOL_PPOL0
#define  left0_center1_alignment  PWMCAE_CAE0
#define  pwm_per_out PWMPER0
#define  pwm_dc_out PWMDTY0
#define  pwm_counter_ch0 PWMCNT0
#define  pwm_reg_en_ch0 PWME_PWME0

#define  per_fmin 4352  // =f_clk/f_in    min (in numero di cicli di f_clk)
#define  per_fmax 255   // =f_clk/f_in    max
#define  dc_fmax  191   // =255*75%
#define  dc_fmin  64    // =255*25%

         
int old_var, new_var, var, cicli_per;


void TIM_interrupt_init_pwm(void) {
   
   //input capture - setup 
   prescale=1; //set il prescale a 1 to run at 3.125MHz
   confA=1;  //set la sensibilità del capture su any edge
   confB=1;  
   arm_ch_0=1; //attivo la generazione di interrupt
   time_count_reg_en=1; //abilito il time counter register

	// PWM part A - setup e configuration
	pwm_clk_en_ch0=0;  //abilito il pwm clk del canale 
	pwm_A0_B1_ch0=0; //scelgo il clka per il ch 0
	pwm_prescal_A=3; //setto il prescaler di clka
	polarity=1;  //imposto la polarity ( inizia alto)
	left0_center1_alignment=0; //scelgo left
	pwm_per_out=255;    // setto il periodo 
  pwm_dc_out=127;     // setto il duty cycle 
  pwm_counter_ch0=0; //resetto il contatore
  pwm_reg_en_ch0=1;   //abilito il registro 

}

void main(void) {
  /* put your own code here */
  
  input_capture_en=0x00; //@RESET, quindi è già configurato come input capture  
  
  TIM_interrupt_init_pwm();
 
	EnableInterrupts;
	
  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

#pragma CODE_SEG NON_BANKED

interrupt 8 void TIM_inter_period_fin()  // 8=(255-238)/2 per difetto 
{
  
  old_var=0;
  
   
  if(signal_wave) 
  {
     new_var=time_counter_stored;
     
     if(old_var>new_var) 
     {
       cicli_per=0xFFFF-old_var+new_var;
       
     } 
     else 
     {
       cicli_per=new_var-old_var;
     }
     
     old_var=new_var; //aggiorno old_var per il next state
  } 
  else 
  {
    var=time_counter_stored;
    
  }
     
     
  flag_ch_0=1;
   
   
  if(cicli_per>per_fmax) {
  
  
    cicli_per=per_fmax;
    
  }
  
  if(cicli_per<per_fmin){
    
  
  cicli_per=per_fmin;
 
  
  }
  
  pwm_dc_out=-cicli_per/32+199;    //equazione che implementa d=f(freq);
 

}
#pragma CODE_SEG DEFAULT