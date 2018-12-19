#include <msp430.h>

// Debouncing for G2,

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
	
  P1DIR |= BIT6;                  // setting P1.6 output direction to 1
  P1OUT |= BIT3;                  // setting P1.3 to 1
  P1REN |= BIT3;				          // enabling pull-up resister on P1.3
  P1IE  |= BIT3;    			        // enabling interrupt on P1.3
  P1IES |= BIT3;                  // setting P1.3 on high/low edge
  P1IFG &= ~BIT3;                 // clears interrupt flag on P1.3

  TA0CCTL0 = CCIE;                // enabling CCR0 interrupt
  TA0CCR0  = 20000;               // reseting timer A to 20000

  _BIS_SR(LPM0 + GIE);            // setting to Low Power Mode with global interrupt enable. 

  while(1){ }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    P1IE  |= BIT3;                  // enabling P1.3 interrupt
    TA0CTL = 0x0000;                // disabling and reseting timer A
    P1IFG &= ~BIT3;                 // disables interrupt flag
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CTL = TASSEL_1 + MC_1;       // setting timer A to counting up mode
    P1IE  &= ~BIT3;                 // disabling interrupt on P1.3
    P1IFG &= ~BIT3;                 // disabling interrupt flag on P1.3
    P1OUT ^= BIT6;                  // toggle P1.6 LED
}

