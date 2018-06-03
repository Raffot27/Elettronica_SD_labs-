#include <hidef.h> /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#define N0 2  //250
#define N1 2  //250 per 2khzcon dc=50%

#define ONDA PTM_PTM1 

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


void main(void)
{
  /* put your variable declarations here */
  int up_down=0;
  int i,j;
  
  DDRM=0x02;  //abilito come uscita ptm1
  PTM=0x00;
  
  CPMUPOSTDIV_POSTDIV = 1; // set POSTDIV
  // wait PLL lock
  

while (!CPMUFLG_LOCK);
/* put your initializations here */

TIM_init();
TIM_interrupt_init();

EnableInterrupts;

/* never ending loop */
for(;;)
  {

    /* put your code here */
   
   if (up_down) 
   {
      ONDA=1;
      for(i=0; i<N1; i++);
      up_down=0;
   } 
   else {
    
   ONDA=0;
   for(j=0; j<N0; j++);
   up_down=1;
    
   }
    
    

  _FEED_COP(); /* feeds the dog */
  } /* loop forever */
}
#pragma CODE_SEG NON_BANKED
interrupt 9 void TIM_int1(void)
{
static int x = INT1_INIT_VAL;
x = (x >> 2) | (((x & 1)^(x & 2)) << 11);
TC1 += x;
}
#pragma CODE_SEG DEFAULT