#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define onda PTM_PTM1
#define delta_mezzi 195 //faccio tutto il calcolo delta=T_voluta/T_clk
#define equal TFLG1_C3F //sarebbe il flag di comparazione del canale 3
#define soglia TC3 //la comparazione � fatta sul canale 3
#define risoluzione ATDCTL1_SRES0
#define giustificazione ATDCTL3_DJM
#define scan_loop ATDCTL5_SCAN
#define pot_conv_flag ATDSTAT2L_CCF0
#define pot_conv ATDDR0
#define delta_max 448 //calcolo il delta per avere una f min di 870 hz
#define INT1_INIT_VAL 500

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


void main(void) {

   int delta_var;
  
  /* put your own code here */
  CPMUPROT=0x00; // disabilito la protezione del registro di configurazione
  CPMUOSC=0x00; //scelgo l'internal oscillatore mettendo a 0 l'msb,se lo metto a uno scelgo l'oscillatore esterno
  CPMUCLKS=0x80; //seleziono il PLL come sorgente di clock
  DDRM=0x02;
  //when the bit LOCK in CPMUFLG is �1� the clock signal generated by the PLL is stable and can be used.
  //f_pll=12.5 MHZ , con SYNDIV=25 POSTDIV=3
  
  


  
  onda=1;
  CPMUPOSTDIV_POSTDIV=1;  //setto postdiv=1
  
  while (!CPMUFLG_LOCK); //finch� non diventa stabile il segnale di clock
  
  TIM_init();
  TIM_interrupt_init();
  
  EnableInterrupts;
  
  TSCR1=0x90; // Abilitiamo il tcnt (TEN) e il tffca (fast flag clear abilitato)
  TSCR2=0x04; // settiamo il PR=4
  TIOS=0x08; //attivo IOS3 per abilitare l'output compare 3
  
  soglia=TCNT+delta_mezzi;
  
  //SETTAGGGIO ATD
  risoluzione=1; //imposto la risoluzione del conv a 10 bit (al reset � a 8 bit)
  giustificazione=1;//set in modo da avere l'lsb nella posizione 0
  scan_loop=1; //abilito la conversione in scansione continua
  
  for(;;) 
  {
    if(equal) 
    {
      onda^=1;
      
      if(pot_conv_flag) 
      {
        
        delta_var=delta_mezzi+pot_conv;
      }
      
      if(delta_var>delta_max)  //imposto la fmin a 870 Hz
      {
       delta_var=delta_max;
      }
      soglia=TCNT+delta_var;
    }
  
  
 
  
  
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
