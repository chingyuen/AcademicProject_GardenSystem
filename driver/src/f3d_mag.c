/* f3d_mag.c --- 
 * 
 * Filename: f3d_mag.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct 31 09:27:11 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */


#include <f3d_i2c.h>
#include <f3d_mag.h>
#include <math.h>

#define PI 3.1415927

void f3d_mag_init() {
    // MAG I2C Address = 0x3C 

    uint8_t value = 0; 

    value = 0x14;                  //Temp sensor disable,30Hz Output Rate 
    f3d_i2c1_write(0x3C, 0x00,  &value); // Mag (0x3C), CRA (0x00) 

    value = 0xE0;                      // +/- 8.1 Gauss Full Scale
    f3d_i2c1_write(0x3C, 0x01, &value); // Mag (0x3C), CRB (0x01)

    value = 0x00;                      // Continuous Conversion
    f3d_i2c1_write(0x3C, 0x02, &value); // Mag (0x3C), MR  (0x23)
}

float f3d_calculate_direction(float *pitch_roll_yaw, float *mag_data) {
    
    float Pitch = pitch_roll_yaw[0], Roll = pitch_roll_yaw[1];
    float XH,YH,heading;

    XH=mag_data[0]*cos(Pitch)+mag_data[2]*sin(Pitch);
    YH=mag_data[0]*sin(Roll)*sin(Pitch)+mag_data[1]*cos(Roll)-mag_data[2]*sin(Roll)*cos(Pitch);

    heading=atan2(YH,XH)*180/PI;
    heading += 180;
    return heading;
}

void f3d_mag_read(float *mag_data) {
    uint8_t buffer[6];
    int i;

    f3d_i2c1_read(0x3C, 0x03, buffer,2);   // Read X Axis
    f3d_i2c1_read(0x3C, 0x07, buffer+2,2); // Read Y Axis
    f3d_i2c1_read(0x3C, 0x05, buffer+4,2); // Read Z Axis (notice that Z is out of order in the chip). 

    for (i=0; i<2; i++) {
        mag_data[i]=(float)((int16_t)(((uint16_t)buffer[2*i] << 8) + buffer[2*i+1]))/230;
    }
    mag_data[2]=(float)((int16_t)(((uint16_t)buffer[4] << 8) + buffer[5]))/205;
}





/* f3d_mag.c ends here */
