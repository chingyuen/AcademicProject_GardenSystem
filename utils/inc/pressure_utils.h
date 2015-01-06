/* pressure_utils.h ---
*
* Filename: pressure_utils.h
* Description: Utilities for LPS331AP Pressure Sensor
* Author: Brian Rak, Ching Ng
* Maintainer: Rak, Ng
* Created: Tue 2-11-14
  * Version: 1.0
  * Last-Updated: Brian Rak
  *           By:
  *     Update #: 1
  */

void convert_to_altitude_ft(float* pressure_mb, float *altitude_ft);
void convert_to_altitude_ft_dp(double* pressure_mb, double *altitude_ft);
void feet_to_meters(float *ft, float *m);
void feet_to_meters_dp(double *ft, double *m);

