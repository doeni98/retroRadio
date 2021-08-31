#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "i2c.h"

int main(void){
        /* Init i2c for scan top */
        int i2cfd = open_i2c("/dev/i2c-1", 0x48);
        if (i2cfd == -1) {
                printf("Failed to init i2c for back scanners\n");
                return -1;
        }

        while (1){
                float volt_0 = detection_volt(0, i2cfd);
                float volt_1 = detection_volt(1, i2cfd);
                float volt_2 = detection_volt(2, i2cfd);

                printf("%4f, %4f, %4f\r", volt_0, volt_1, volt_2);
                fflush(stdout);
                usleep(1000 * 1000);
        }
}




