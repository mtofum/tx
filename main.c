#include "msp430.h"
#include "uart.h"

void bc(void)
{
  unsigned int i;
  BCSCTL1 |= XTS;                           // LFXT1 = HF XTAL
   BCSCTL3 |= LFXT1S1;                       // LFXT1S1 = 3-16Mhz

   do
   {
     IFG1 &= ~OFIFG;                         // Clear OSCFault flag
     for (i = 0xFF; i > 0; i--);             // Time for flag to set
   }
   while (IFG1 & OFIFG);                     // OSCFault flag still set?
   BCSCTL2 |= SELM_3 + SELS;                        // MCLK= LFXT1 SMCLK=LFXT1
}

void pwm_init(void)
{
    P1DIR |= BIT2 + BIT3;                            // P1.2 and P1.3 output
    P1SEL |= BIT2 + BIT3;                            // P1.2 and P1.3 TA1/2 otions
    TA0CTL|=TACLR;
    TA0CCR0 = 200 - 1;                         // PWM Period
    TA0CCTL1 = OUTMOD_3;                       // TA0CCR1 set/reset
    TA0CCR1 = 90;                             // TA0CCR1 PWM duty cycle
    TA0CCTL2 = OUTMOD_7;                       // TA0CCR1 reset/set
    TA0CCR2 = 90;                             // TA0CCR1 PWM duty cycle
    TA0CTL = TASSEL_1;                  // ACLK, up mode
}

void TIM_init(void)
{
    TA1CTL|=TACLR;
    TA1CCR0 = 60000 - 1;                         // PWM Period
    TA1CCTL1 |= CCIE;                       // TA0CCR1 set/reset
    TA1CCR1 = 600;
    TA1CTL = TASSEL_1 + MC_1+ID_3 + TAIE;    // ACLK, up mode
    _EINT();
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void TA1_ISR(void)
{
    switch(TA1IV)
    {
    case 0x02:
        P1OUT|=BIT0;
        TA0CTL&=~MC_1;  //stop timer
        TA0R=0;         //clear timer's counter
        P1DIR &=~(BIT2+BIT3); //stop pin output
        break;
    case 0x0a:
        P1OUT&=~BIT0;
        P1DIR |=(BIT2+BIT3); //start pin output
        TA0CTL|=MC_1;
        break;
    default:
        break;
    }
}

int main (void)
{
    WDTCTL = WDTPW + WDTHOLD;
    bc();
    P1DIR|=BIT0;
    P1OUT&=~BIT0;
    pwm_init();
    TIM_init();
    UART_Init();
    while(1)
    {
        if(uart_data.data_ready==1)
        {
            uart_data.data_ready=0;
        }
    }
    return 0;
}
