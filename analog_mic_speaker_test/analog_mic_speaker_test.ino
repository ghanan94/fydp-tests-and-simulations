#include <stdint.h>

#define LED_PIN 13
#define MIC_PIN 0
#define SPEAKER_PIN DAC1

volatile uint16_t counter;
volatile uint16_t mic_data;
volatile uint8_t on_status;

void setup()
{
    pinMode( LED_PIN, OUTPUT );
    
    // Setup serial baud-rate
    Serial.begin( 9600 );

    /*********************************TIMER*************************/
    counter = 0;
    on_status = true;

    // Disable write protect of PMC registers.
    pmc_set_writeprotect( 0 );

    // Enable clock to TC0.
    if ( pmc_enable_periph_clk( ID_TC0 ) )
    {
        Serial.println( "ID_TC0 peripheral clock setting failed" );
    }

    NVIC_ClearPendingIRQ( TC0_IRQn );
    NVIC_EnableIRQ( TC0_IRQn );
    
    // Set up the Timer in waveform mode which creates a PWM
    // in UP mode with automatic trigger on RC Compare
    // and sets it up with CLOCK1 as clock (84Mhz/2).
    
    TC_Configure( TC0, 0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1 );

    // Reset counter and fire interrupt when RC value is matched.
    // RC Value calculated as Effective clock rate / Timer frequency 
    // = (84000000 / 2) / 32000 = 1312.5.
    TC_SetRC( TC0, 0, 1312 );

    // Enable the RC Compare Interrupt...
    TC0->TC_CHANNEL[ 0 ].TC_IER = TC_IER_CPCS;
    
    // ... and disable all others.
    TC0->TC_CHANNEL[ 0 ].TC_IDR =~ TC_IER_CPCS;
    
    TC_Start( TC0, 0 );

    digitalWrite( LED_PIN, on_status );


    /**************************MIC************************/
    analogReadResolution( 12 );
    mic_data = 0;


    /*************************SPEAKER*********************/
    analogWriteResolution( 12 );
    pinMode( SPEAKER_PIN, OUTPUT );
}

void loop()
{
    Serial.println( mic_data );
    //analogWrite( SPEAKER_PIN, mic_data << 2 );
}

void TC0_Handler( void )
{
    TC_GetStatus( TC0, 0 );

    // Counter simply for led blinking every second.
    if ( ++counter == 8000 )
    {
        counter = 0;
        on_status = !on_status;
        
        digitalWrite( LED_PIN, on_status );
    }

    mic_data = analogRead( MIC_PIN );
    analogWrite( SPEAKER_PIN, mic_data );
}

