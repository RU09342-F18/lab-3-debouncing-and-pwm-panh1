#include <msp430.h>

// Software PWM for G2,

//Variables
#define button_press (P1IN & BIT3)  // Defining button_pressed as (P1IN & BIT3)

void main(void){
	WDTCTL  = WDTPW | WDTHOLD;	      // Stop watchdog timer
  
  // Setting up GPIO
  P1DIR |= BIT0;                    // Selecting P1.0 as output
  P1SEL |= BIT0;                    // Selecting P1.0 to GPIO
  P1OUT |= BIT3;
  P1REN |= BIT3;                    // Enabling pull-up resistor
  P1IE  |= BIT3;                    // Enabling interrupt on P1.3
  P1IES |= BIT3;                    // Selecting interrupt edge
  P1IFG &= ~BIT3;                   // Clearing interrupt flag on P1.3

  // Setting up Timer0
  TA0CTL |= TASSEL_2 + MC_1;        // Selecting SMCLK and Up Mode 
  TA0CCTL0 = CCIE;                  // CCR0 interrupt enabled
  TA0CCR0 = 1000;                   // register that timer will be compare to
  TA0CCR1 = 500;

  __bis_SR_register(GIE);           // Enabling Global Interupt
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void){
  if (button_press == 0){           // If button is pressed
    P1IES &= ~BIT3;                 // Setting interupt to trigger on when button is release
    P1OUT^= BIT0;                   // Toggle LED
  } else {                          // If button is released
    P1IES |= BIT3;                  // Sets interupt to trigger on when button is press
    P1OUT ^= BIT0;                  // Toggle led
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
  P1OUT ^= BIT0;                    // Toggles LED when interupt fires
  TA0CCTL0 &= ~BIT0;                // Clears the interupt flag
}
