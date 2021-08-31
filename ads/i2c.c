#include <stdio.h>
#include <fcntl.h>         // open
#include <inttypes.h>      // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "i2c.h"

int open_i2c(const char *filename, int i2cAddr) {

    int I2CFile;
    int err;

    I2CFile = open(filename, O_RDWR);

    if (I2CFile == -1) {
        return -1;
    }

    err = ioctl(I2CFile, I2C_SLAVE, i2cAddr);
    if (err != 0) {
        return -1;
    }

    return I2CFile;
}

void close_i2c(int i2c_file_fd) {

    close(i2c_file_fd);
}

float detection_volt(int num_analog_entry, int i2c_file_fd) {

    uint8_t writeBuf[3];		// Buffer to store the 3 bytes that we write to the I2C device
    uint8_t readBuf[2];		// 2 byte buffer to store the data read from the I2C device

    int16_t val; // Stores the 16 bit value of our ADC conversion

    // These three bytes are written to the ADS1115 to set the config register and start a conversion
    writeBuf[0] = 1;		// This sets the pointer register so that the following two bytes write to the config register
    writeBuf[1] = 0xC3 | (num_analog_entry << 4);   // This sets the 8 MSBs of the config register (bits 15-8) to 11000011 (0xC3 = A0, 0xD3 = A1, 0xE3 = A2, 0xF3 = A3)
    writeBuf[2] = 0x03;  	// This sets the 8 LSBs of the config register (bits 7-0)  to 00000011

    // Initialize the buffer used to read data from the ADS1115 to 0
    readBuf[0]= 0;
    readBuf[1]= 0;

    // Write writeBuf to the ADS1115, the 3 specifies the number of bytes we are writing,
    // this begins a single conversion
    write(i2c_file_fd, writeBuf, 3);

    // Wait for the conversion to complete, this requires bit 15 to change from 0->1
    while ((readBuf[0] & 0x80) == 0) // readBuf[0] contains 8 MSBs of config register, AND with 10000000 to select bit 15
    {
        read(i2c_file_fd, readBuf, 2); // Read the config register into readBuf
    }

    writeBuf[0] = 0; // Set pointer register to 0 to read from the conversion register
    write(i2c_file_fd, writeBuf, 1);

    read(i2c_file_fd, readBuf, 2); // Read the contents of the conversion register into readBuf

    val = readBuf[0] << 8 | readBuf[1];	// Combine the two bytes of readBuf into a single 16 bit result

    return (float)val * 4.096/32767.0;
}
