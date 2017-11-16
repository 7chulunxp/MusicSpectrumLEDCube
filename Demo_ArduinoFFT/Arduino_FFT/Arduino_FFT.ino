/*
  fft_adc.pde
  guest openmusiclabs.com 8.18.12
  example sketch for testing the fft library.
  it takes in data on ADC0 (Analog0) and processes them
  with the fft. the data is sent out over the serial
  port at 115.2kb.  there is a pure data patch for
  visualizing the data.
*/

// do #defines BEFORE #includes
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

int bin = 0;
byte frequency[128];
int resolution = 9615 / 256;
boolean inRange = false;

void setup() {
  Serial.begin(9600); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter - delay() and millis() killed
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //set adc prescaler=128
  ADMUX = 0x40; // use adc0
  ADCSRA |= (1 << ADATE); //enable auto trigger
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements

  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
  while (1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      while (!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA |= (1 << ADSC); //start ADC measurements
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i + 1] = 0; // set odd bins to 0
    }
    // window data, then reorder, then run, then take output
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei(); // turn interrupts back on
    //Serial.write(255); // send a start byte
    //Serial.write(fft_log_out, 128); // send out the data
    for (int i = 0; i < 128; i++) {
      frequency[i] = fft_log_out[i] * resolution;
      //Serial.print("||frequencyMR.Bin||");
      //Serial.println(frequency[i]);
    }//end of for

    //    Serial.write(11111);
    Serial.write(frequency, 128);
    //    //Serial.println("New_Line==================");
    //       for(int i = 0; i < 128; i++){
    //         Serial.println(frequency[i]);
    //     }
  }
}