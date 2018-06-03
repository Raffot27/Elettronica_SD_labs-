#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */ 

#define INT1_INIT_VAL 500 
#define delta_mezzi 195
#define delta_max 449 //relativa alla freq max

#define ONDA PTM_PTM1
 
#define protezione CPMUPROT_PROT
#define osc_int CPMUOSC_OSCE
#define sorgente_clk CPMUCLKS_PLLSEL
#define enable_reg_comp TIOS_IOS3
//#define flag_reg_comp TFLG1_C3F
#define reg_comp TC3

#define risoluzione ATDCTL1_SRES0
#define flag_fine_conv ATDSTAT2_CCF0
//noi stiamo collegando il potenziometro al canale 0-->quindi il valore convertito sarà salvato in ATDDR0

#define en_interr_ch3 TIE_C3I //bit legato all'enable del canale 3 del timer interrupt enable (TIE)


void TIM_init()
{
  TSCR1_TEN = 1; // enable the counter
  TSCR2_PR = 4; // set the prescaler    
} 
 
/* DO NOT MODIFY THIS FUNCTION !! */ 
void TIM_interrupt_init()
{   
    TSCR1_TFFCA = 1; // set fast flag clear: updating TC1 resets TFLG1_C1F   
    TIOS_IOS1 = 1; // set channel 1 as output compare   
    TC1 = TCNT + INT1_INIT_VAL; // set channel 1 threshold  
     TIE_C1I = 1; // enable interrupt generation on channel 1 
}   
 


void TIM_interrupt_init3()  // 11=(255-232)/2
  {  
 
     en_interr_ch3 = 1;
  }
 
 int delta_variabile;
 
 void main(void)  
 { 
     
    
     protezione = 0;
     osc_int = 0;
     sorgente_clk = 1;
     
     DDRM = 0x02; //PTM1 out
     ONDA =1;
     
     CPMUPOSTDIV = 0x01; //setto il postdiv a 1
     
      //wait PLL lock
    while (!CPMUFLG_LOCK); //serve per aspettare che il PLL sia stabile
     TIM_init();        
     TIM_interrupt_init(); 
     TIM_interrupt_init3();  
     
     TSCR1= 0x90; //abiliatiamo il TCNT e il fast flag clear--> che si attiva ogni volta che cambiano reg_comp(TC3) oppure il flag(C3F)
     TSCR2 = 0x04; //set il prescaler a 4 
     
     enable_reg_comp = 1; //abilito il canale 3 per fare le comparazioni
    // flag_reg_comp = 1; //attivo il flag di reg_comp
     
     risoluzione = 1; //imposto la risoluzione a 10 bit
     ATDCTL3_DJM = 1; //imposto l'LSB al bit in posizione 0
     ATDCTL5_SCAN = 1; //abilito la conversione in maniera continua
     
     //Configurazione riguardante il lab 9
     en_interr_ch3 = 1; //abilito l'interrupt del canale 3, ovvero di reg_comp (TC3)
     
     //noi stiamo collegando il potenziometro al canale 0-->quindi il valore convertito sarà salvato in ATDDR0
    
      
    
     delta_variabile = delta_mezzi + ATDDR0; //sommo a delta_mezzi il valore converito del potenziometro in modo tale da avere una delta variabile
     reg_comp= TCNT + delta_variabile; 
        
   
 
  EnableInterrupts; 
 


 for(;;)
   { 
     // while(!flag_reg_comp); //aspetta finchè non avviene la comparazione
      
      
               
      _FEED_COP(); /* feeds the dog */ 
    } /* loop forever */ 
  }
  
  
  
  
    
  
  
  #pragma CODE_SEG NON_BANKED
  
  interrupt 11 void routine_init3(void)  // 11=(255-232)/2
  {  
     ONDA ^=1;
      
      if(flag_fine_conv == 1) {
        delta_variabile = delta_mezzi + ATDDR0;
      }
      
      if(delta_variabile > delta_max) 
      { 
      delta_variabile = delta_max;
      } 
      
      if(delta_variabile <delta_mezzi) 
      {
        delta_variabile = delta_mezzi;
      }
        
      
      reg_comp= TCNT + delta_variabile;  
  
  }
  
  
  
   
interrupt 9 void TIM_int1(void)  
{   
    static int x = INT1_INIT_VAL;   
    x = (x >> 2) | (((x & 1)^(x & 2)) << 11);   
    TC1 += x;  
  } 
   #pragma CODE_SEG DEFAULT 
