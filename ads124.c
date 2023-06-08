ads124
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define ADS124S80_CS_PIN  25

// SPI channel and speed
#define SPI_CHANNEL       0
#define SPI_SPEED         1000000  // 1MHz

// Register addresses
#define REG_STATUS        0x00
#define REG_MUX           0x01
#define REG_ADCON         0x02
#define REG_DRATE         0x03
#define REG_IO            0x04
#define REG_OFC0          0x05
#define REG_OFC1          0x06
#define REG_OFC2          0x07
#define REG_FSC0          0x08
#define REG_FSC1          0x09
#define REG_FSC2          0x0A

// Function to write a value to a register
void ads124s80_write_register(uint8_t reg, uint8_t value) {
  digitalWrite(ADS124S80_CS_PIN, LOW);
  wiringPiSPIDataRW(SPI_CHANNEL, &reg, 1);
  wiringPiSPIDataRW(SPI_CHANNEL, &value, 1);
  digitalWrite(ADS124S80_CS_PIN, HIGH);
}

// Function to read a value from a register
uint8_t ads124s80_read_register(uint8_t reg) {
  reg |= 0x40;  // Set the read bit
  digitalWrite(ADS124S80_CS_PIN, LOW);
  wiringPiSPIDataRW(SPI_CHANNEL, &reg, 1);
  uint8_t value;
  wiringPiSPIDataRW(SPI_CHANNEL, &value, 1);
  digitalWrite(ADS124S80_CS_PIN, HIGH);
  return value;
}

int main() {
  // Setup WiringPi
  wiringPiSetup();
  wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
 
  // Configure the ADS124S80
  pinMode(ADS124S80_CS_PIN, OUTPUT);
  digitalWrite(ADS124S80_CS_PIN, HIGH); // Set CS pin high initially
 
  // Set the MUX register to select channel 0 and configure for bipolar operation
  uint8_t muxValue = 0x10; // Channel 0, bipolar operation
  ads124s80_write_register(REG_MUX, muxValue);
 
  // Set the ADCON register to set the gain and reference voltage
  uint8_t adconValue = 0x20; // Gain 1, internal reference
  ads124s80_write_register(REG_ADCON, adconValue);
 
  // Set the DRATE register to set the data rate
  uint8_t drateValue = 0x0F; // 20 SPS (Samples Per Second)
  ads124s80_write_register(REG_DRATE, drateValue);
 
  // Read the status register to check if the configuration is successful
  uint8_t status = ads124s80_read_register(REG_STATUS);
  if (status == 0x00) {
    printf("ADS124S80 configured successfully!\n");
  } else {
    printf("ADS124S80 configuration failed. Status: 0x%02X\n", status);
    return 1;
  }
 
  // Read the converted data
  while (1) {
    uint8_t msb = ads124s80_read_register(RER

  // Read the converted data
  while (1) {
    uint8_t msb = ads124s80_read_register(REG_IO);
    uint8_t lsb = ads124s80_read_register(REG_IO);


   
    // Combine the MSB and LSB to get the 16-bit ADC value
    uint16_t adcValue = (msb << 8) | lsb;
   
    // Print the ADC value
    printf("ADC Value: %d\n", adcValue);
   
    // Delay before reading again (adjust as needed)
    delay(1000); // 1 second delay
  }
 
  return 0;
}



#define CMD_START  0x08
#define CMD_STOP   0x0A

// ...

int main() {
  // ...

  // Start conversions
  ads124s80_write_register(REG_STATUS, CMD_START);

  // Read the converted data
  while (1) {
    uint8_t msb = ads124s80_read_register(REG_IO);
    uint8_t lsb = ads124s80_read_register(REG_IO);
   
    // Combine the MSB and LSB to get the 16-bit ADC value
    uint16_t adcValue = (msb << 8) | lsb;
   
    // Print the ADC value
    printf("ADC Value: %d\n", adcValue);
   
    // Delay before reading again (adjust as needed)
    delay(1000); // 1 second delay
  }
 
  // Stop conversions
  ads124s80_write_register(REG_STATUS, CMD_STOP);

  return 0;
}