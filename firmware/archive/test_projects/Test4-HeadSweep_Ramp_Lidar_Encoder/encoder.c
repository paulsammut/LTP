#include "encoder.h"
#include "mcc_generated_files/mcc.h"
#include "LTP_system.h"
#include <stdio.h>

void encoder_init(void) {

    // Here we set the registers of the LS7366R chip to 
    // do a reset of the count after passing the index
    SS_ENCODER = 0;
    MSSP1_SPI_Exchange8bit(0x88); // Write to MRD0
    MSSP1_SPI_Exchange8bit(0x23); // x4 quadrature, Index resets CNTR
    SS_ENCODER = 1;

}

/*  This function does an SPI read of the CNTR register which is a 4 byte register. 
 *  In it, is the current angle of the LIDAR head. 
 */
int16_t getAngle(void) {

    int16_t readData;
    uint8_t LSByte;
    uint8_t MSByte;

    // The LS7366R is a 32bit counter chip which means 4 bytes. 
    // We only care about the last two bytes because we are only counting
    // up to 4000. 
    SS_ENCODER = 0;
    MSSP1_SPI_Exchange8bit(0x60);
    MSSP1_SPI_Exchange8bit(0x00);
    MSSP1_SPI_Exchange8bit(0x00);
    MSByte = MSSP1_SPI_Exchange8bit(0x00);
    LSByte = MSSP1_SPI_Exchange8bit(0x00);
    SS_ENCODER = 1;
    
    // We shift over the MSB and insert the LSB
    readData = MSByte << 8 | LSByte;
    
    // If we go negative, meaning a counter clockwise cross of the index, 
    // do some math and give us the positive number
    if(readData < 0)
        readData = 3999 + readData;
    
    return readData;
}

void testEncoder(void) {
    SS_ENCODER = 0;
    MSSP1_SPI_Exchange8bit(0x48);
    uint8_t rdata = MSSP1_SPI_Exchange8bit(0x00);
    SS_ENCODER = 1;
    printf("Encoder is posting: 0x%02x",rdata);
}


