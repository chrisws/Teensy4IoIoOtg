/*
 * Original code by Ytai Ben-Tsvi, part of the IOIO-OTG project
 * https://github.com/ytai/ioio
 *
 * Modified by Chris Warren-Smith
 * These modifications were made to port the IOIO-OTG firmware to Teensy hardware.
 *
 * License: [Apache License, Version 2.0]
 * See the LICENSE file for full details.
 *
 * Portions of this code may have been rewritten or replaced entirely.
 */

#include "adc.h"

#if 0
#include <ADC.h>

// Create an ADC object for Teensy's ADC peripheral
ADC *adc = nullptr;

void ADCInit() {
  // Initialize the ADC object
  adc = new ADC();

  // Set 12-bit resolution for ADC0 and ADC1
  adc->adc0->setResolution(12);
  adc->adc1->setResolution(12);

  // Configure 16-sample averaging
  adc->adc0->setAveraging(16);
  adc->adc1->setAveraging(16);

  // Set high-speed conversion
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);

  // Set high-speed sampling
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);
}

void ADCSetScan(int pin, int enable) {
  if (enable) {
    // Start an ADC scan on the specified pin
    int analog_value = adc->analogRead(pin);
    Serial.printf("ADC Scan on pin %d: %d\n", pin, analog_value);
  } else {
    // Disable scanning (optional functionality)
    Serial.printf("ADC Scan disabled on pin %d\n", pin);
  }
}

void ADCSetCapSense(int pin, int enable) {
  if (enable) {
    // Configure the pin for capacitive sensing and start a read
    // Read differential for capacitive sensing
    int cap_value = adc->analogReadDifferential(pin, 0);
    Serial.printf("Capacitive Sensing on pin %d: %d\n", pin, cap_value);
  } else {
    // Disable capacitive sensing (optional functionality)
    Serial.printf("Capacitive Sensing disabled on pin %d\n", pin);
  }
}
#endif

#include "adc.h"

// Include Teensy 4 hardware definitions for direct register access
#include "imxrt.h"

// Initialize the ADC system
void ADCInit() {
  // Enable ADC clock for ADC1
  CCM_CCGR1 |= CCM_CCGR1_ADC1(CCM_CCGR_ON);

  // Configure the ADC1 for differential mode, 16-bit resolution
  ADC1_CFG |= ADC_CFG_ADSTS(0b11);  // Set sample time
  ADC1_CFG |= ADC_CFG_MODE(0b11);   // Set 16-bit differential mode
}

// Set or unset a pin for scanning (single-ended or differential)
void ADCSetScan(int pin, int enable) {
  if (enable) {
    // Configure the specific pin for differential mode if needed.
    // You can adjust this for a specific pin pair like A0-A1
    // Here is an example for A0-A1:
    if (pin == 0) { // Assuming pin 0 represents A0-A1 pair for differential
      ADC1_HC0 = ADC_HC_ADCH(0b00010);  // Channel for A0-A1 differential pair
    } else {
      // Implement logic for other differential pin pairs, or error handling
    }
  } else {
    // Handle disabling scan mode if needed (might just be a no-op)
  }
}

// Enable capacitive sensing on a pin (specific to your application)
void ADCSetCapSense(int pin, int enable) {
  // Teensy 4 does not natively support capacitive sensing on all pins like previous models.
  // Capacitive sensing is typically done on specific pins and involves separate hardware configurations.
  // Assuming this function handles pins dedicated to cap sensing:

  if (enable) {
    // Enable capacitive sensing on the given pin (adjust pin mapping as needed)
    // This part will depend on which hardware features you're using for cap sensing.
  } else {
    // Disable capacitive sensing on the given pin
  }
}

// Function to read a differential ADC value (for internal use)
int16_t readDifferentialADC() {
  // Trigger a conversion for differential pair (A0-A1 as an example)
  ADC1_HC0 |= ADC_HC_ADCH(0b00010);  // Adjust based on the differential pair

  // Wait for conversion to complete
  while (!(ADC1_HS & ADC_HS_COCO0));

  // Return the signed 16-bit result
  return (int16_t)(ADC1_R0 & 0xFFFF);
}
