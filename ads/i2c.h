//
// Created by deni on 27/04/2021.
//

#ifndef STRATEGIE_I2C_H
#define STRATEGIE_I2C_H

int open_i2c(const char *filename, int i2cAddr);
float detection_volt(int num_analog_entry, int i2c_file_fd);
void close_i2c(int i2c_file_fd);

#endif //STRATEGIE_I2C_H
g