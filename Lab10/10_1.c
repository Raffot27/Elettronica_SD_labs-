#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define signal_wave PTT_PTT0   //l'input dell' onda che ricevo

#define LED1 PTT_PTT4
#define LED2 PTT_PTT5
#define LED3 PTT_PTT6
#define LED4 PTT_PTT7

#define input_capture_en TIOS
#define prescale TSCR2_PR
#define arm_ch_0 TIE_C0I
#define time_count_reg_en TSCR1_TEN
#define flag_ch_0  TFLG1_C0F
#define time_counter TCNT
#define time_counter_stored TC0
#define delta_1 3125
#define delta_2 1563
#define delta_3 781                
#define  confA TCTL4_EDG0A  //mi configurano a cosa sarà sensibile il capture 
#define  confB TCTL4_EDG0B



unsigned int dc; //globale perchè devo salvarmi il dato //duty cicle
unsigned int T, TH, f; // periodo e parte alta del periodo
int old_var, new_var, var;

void TIM_interrupt_init0(void) 
{
  //PART A
   
    DDRT= 0xF0; //abilito i led come output e lascio come input l'ic
    LED1=1; //spengo tutti i led @inizio
    LED2=1;
    LED3=1;
    LED4=1;
    
    prescale=1; //set il prescale a 1
    
    confA=1;  //set la sensibilità del capture su any edge
    confB=1; 
    
    arm_ch_0=1; //attivo la generazione di interrupt
    time_count_reg_en=1; //abilito il time counter register
      
}

void main(void) {
  /* put your own code here */
  
  input_capture_en=0x00; //@RESET, quindi è già configurato come input capture  
  
  TIM_interrupt_init0();
 
	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

#pragma CODE_SEG NON_BANKED

interrupt 8 void TIM_inter0()  // 8=(255-238)/2 per difetto 
{
  
  old_var=0;
   
  if(signal_wave) 
  {
     new_var=time_counter_stored;
     
     if(old_var>new_var) 
     {
       T=0xFFFF-old_var+new_var;
       
     } 
     else 
     {
       T=new_var-old_var;
     }
     
     old_var=new_var; //aggiorno old_var per il next state
  } 
  else 
  {
    var=time_counter_stored;
    
     if(old_var>var) {
     
     TH=0xFFFF+var-old_var;
     
     } 
     else 
     {
       TH=var-old_var;
     }
    
  }
     
     
  flag_ch_0=1;
     
   dc=TH*100/T;  
   f=1/T;
    //LED MANAGING
    //period
   
   
   if( T>delta_1  ) {    //settaggio led in base al periodo
   
   LED2=1;
   LED1=1;
   }
   
   if( !(T>delta_1) && T>delta_2 ) {
   
   LED2=1;
   LED1=0;
   }
   
   if( !(T>delta_2) && T>delta_3 ) {
   
   LED2=0;
   LED1=1;
   }
   
   if( !(T>delta_3) ) {
   
   LED2=0;
   LED1=0;
   }
 
   //duty cycle
   if( dc<25 ) {    //settaggio led in base al dc
   
   LED4=0;
   LED3=0;
   }
   
   if( !(dc<25) && (dc<50) ) {    
   
   LED4=0;
   LED3=1;
   }
   
   if( !(dc<50) && (dc<75) ) {    
   
   LED4=1;
   LED3=0;
   }
   
   if( !(dc<75) ) {   
   
   LED4=1;
   LED3=1;

   }
}
#pragma CODE_SEG DEFAULT