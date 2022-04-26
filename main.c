#include <msp430.h> 
#include "driverlib.h"


int16_t ADC_Result[4];
int16_t i=3;
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	SYSCFG2=(ADCPCTL2|ADCPCTL3);

	PMM_unlockLPM5();

	ADC_init(ADC_BASE,ADC_SAMPLEHOLDSOURCE_SC,ADC_CLOCKSOURCE_ADCOSC,ADC_CLOCKDIVIDER_1);

	ADC_enable(ADC_BASE);

	ADC_setupSamplingTimer(ADC_BASE,ADC_CYCLEHOLD_16_CYCLES,ADC_MULTIPLESAMPLESDISABLE);

	ADC_configureMemory(ADC_BASE,ADC_INPUT_A2,ADC_VREFPOS_AVCC,ADC_VREFNEG_AVSS);

	ADC_configureMemory(ADC_BASE,ADC_INPUT_A3,ADC_VREFPOS_AVCC,ADC_VREFNEG_AVSS);

	ADC_clearInterrupt(ADC_BASE,ADC_COMPLETED_INTERRUPT);

	ADC_enableInterrupt(ADC_BASE,ADC_COMPLETED_INTERRUPT);

	while(1)
	{
	    __delay_cycles(5000);

	    ADC_startConversion(ADC_BASE,ADC_SEQOFCHANNELS);

	    __bis_SR_register(CPUOFF + GIE);

	    __no_operation();
	}

        return 0;

}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC_VECTOR)))
#endif
void ADC_ISR (void)
{

        switch (__even_in_range(ADCIV,ADCIV_ADCIFG)){
            case  ADCIV_NONE: break; //No interrupt
            case  ADCIV_ADCOVIFG: break; //conversion result overflow
            case  ADCIV_ADCTOVIFG: break; //conversion time overflow
            case  ADCIV_ADCHIIFG: break; //ADCHI
            case  ADCIV_ADCLOIFG: break; //ADCLO
            case  ADCIV_ADCINIFG: break; //ADCIN
            case  ADCIV_ADCIFG:        //ADCIFG0
                ADC_Result[i] = ADC_getResults(ADC_BASE);
                if(i==0)
                {
                    //__no_operation();
                    i = 3;
                }
                else
                {
                    i--;
                }
                __bic_SR_register_on_exit(CPUOFF);
                break;
            default: break;
          }
}



