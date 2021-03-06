#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define onda PTM_PTM1
#define delta_mezzi 195 //faccio tutto il calcolo delta=T_voluta/T_clk

void main(void) {

  
   unsigned int thr;
  
  /* put your own code here */
  CPMUPROT=0x00; // disabilito la protezione del registro di configurazione
  CPMUOSC=0x00; //scelgo l'internal oscillatore mettendo a 0 l'lsb,se lo metto a uno scelgo l'oscillatore esterno
  CPMUCLKS=0x80; //seleziono il PLL come sorgente di clock
  DDRM=0x02;
  //when the bit LOCK in CPMUFLG is �1� the clock signal generated by the PLL is stable and can be used.
  //f_pll=12.5 MHZ , con SYNDIV=25 POSTDIV=3
  
  
	EnableInterrupts;

  
  onda=1;
  CPMUPOSTDIV_POSTDIV=1;  //setto postdiv=1
  
  while (!CPMUFLG_LOCK); //finch� non diventa stabile il segnale di clock
  
  TSCR1=0x80; // Abilitiamo il tcnt (TEN)
  TSCR2=0x04; // settiamo il PR=4
  
  for(;;) 
  {
    
    thr=TCNT+delta_mezzi; //threeshold
    
    if (thr<delta_mezzi) 
    {
      
      while (TCNT>thr);
    } 
    
    while (TCNT<thr);
    onda^=1; //toggle
    
  
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
