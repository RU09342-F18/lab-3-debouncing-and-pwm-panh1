#include <msp430.h>

// Debouncing for 5529,

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	      // stop watchdog timer
	
  P1DIR |= BIT0;                  // setting P1.0 output direction to 1
  P1OUT |= BIT1;                  // setting P1.1 to 1
  P1REN |= BIT1;				          // enabling pull-up resister on P1.1
  P1IE  |= BIT1;    			        // enabling interrupt on P1.1
  P1IES |= BIT1;                  // setting P1.1 on high/low edge
  P1IFG &= ~BIT1;                 // clears interrupt flag on P1.1

  TA0CCTL0 = CCIE;                // enabling CCR0 interrupt
  TA0CCR0  = 20000;               // reseting timer A to 20000

  _BIS_SR(LPM0 + GIE);            // setting to Low Power Mode with global interrupt enable. 

  while(1){  }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    P1IE  |= BIT1;                  // enabling P1.1 interrupt
    TA0CTL = 0x0000;                // disabling and reseting timer A
    P1IFG &= ~BIT1;                 // disables interrupt flag
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CTL = TASSEL_1 | MC_1;       // setting timer A to counting up mode
    P1IE  &= ~BIT1;                 // disabling interrupt on P1.1
    P1IFG &= ~BIT1;                 // disabling interrupt flag on P1.1
    P1OUT ^= BIT0;                  // toggle P1.0 LED
}


