#include <msp430.h>

// Hardware PWM for F5529,

//Variables
#define button_press (P1IN & BIT1)  // Defining button_pressed as (P1IN & BIT1)

void main(void){
	WDTCTL  = WDTPW | WDTHOLD;	      // Stop watchdog timer
  
  // Setting up GPIO
  P1DIR |= BIT0;                    // Selecting P1.0 as output
  P1SEL |= BIT0;                    // Selecting P1.0 to GPIO
  P1OUT |= BIT1;
  P1REN |= BIT1;                    // Enabling pull-up resistor
  P1IE  |= BIT1;                    // Enabling interrupt on P1.1
  P1IES |= BIT1;                    // Selecting interrupt edge
  P1IFG &= ~BIT1;                   // Clearing interrupt flag on P1.1

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
    P1IES &= ~BIT1;                 // Setting interupt to trigger on when button is release
    P1OUT^= BIT0;                   // Toggle LED
  } else {                          // If button is released
    P1IES |= BIT1;                  // Sets interupt to trigger on when button is press
    P1OUT ^= BIT0;                  // Toggle led
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
  P1OUT ^= BIT0;                    // Toggles LED when interupt fires
  TA0CCTL0 &= ~BIT0;                // Clears the interupt flag
}
