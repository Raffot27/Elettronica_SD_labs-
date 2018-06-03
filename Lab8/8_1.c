#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define LED1 PTT_PTT4
#define LED2 PTT_PTT5
#define LED3 PTT_PTT6
#define LED4 PTT_PTT7

#define SW1 PT1AD_PT1AD4
#define SW2 PT1AD_PT1AD5

#define MYWAIT 1  //SI DOVREBBE SETTARE A 20 ms

void main(void) {
  
  int S1=1;
  int S2=0;
  volatile int cnt1;
  volatile int cnt2;
  int i,j;
   
  /* put your own code here */
    
  DDR1AD=0x00; //setto gli sw come input
  DDRT=0xF0;  //set led come output
  PER1AD=0x20; // abilito il pull degli switch
  //PPS1AD=0x00;   pull up si accende a 1 , commentato perchè e una merda il chip e non lo legge
  PTT=0xF0;  //tutti e 4 i led spenti
  
  //ATDDIEN=0x0020;  //abilito il digital input in ad4 e ad5 (FORSE)
  
    
     
	//EnableInterrupts;


  for(;;) 
  {
  
       if(S1) 
    {
      cnt1=!LED1+!LED2*2+!LED3*4+!LED4*8;
      
      cnt1++;
      
      LED1=!(cnt1%2); //prendo il resto
      cnt1=cnt1/2; //aggiorno il contatore
      LED2=!(cnt1%2);
      cnt1=cnt1/2;
      LED3=!(cnt1%2);
      cnt1=cnt1/2;
      LED4=!(cnt1%2);
      cnt1=cnt1/2;
    }
    
    S1^=1; //cambio s1 tanto per provarlo
  
  
    if(S2) 
    {
     for(i=0; i<MYWAIT; i++)
        for(j=0; j<MYWAIT; j++);
        
        cnt2=!LED1+!LED2*2+!LED3*4+!LED4*8;
        
        cnt2++;
        
        
        LED1=!(cnt2%2); //prendo il resto
        cnt2=cnt2/2; //aggiorno il contatore
        LED2=!(cnt2%2);
        cnt2=cnt2/2;
        LED3=!(cnt2%2);
        cnt2=cnt2/2;
        LED4=!(cnt2%2);
        cnt2=cnt2/2;
    }
    
    S2^=1;
  
  
  
  
  
  
  
  
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
