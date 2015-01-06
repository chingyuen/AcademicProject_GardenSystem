/* pressure_utils.c ---
 *
 * Filename: pressure_utils.c
 * Description: Utilities for LPS331AP Pressure Sensor
 * Author: Brian Rak, Ching Ng
 * Maintainer: Rak, Ng
 * Created: Tue 2-11-14
 * Version: 1.0
 * Last-Updated: Brian Rak
 *           By:
 *     Update #: 1
 */

#include <pressure_utils.h>
#include <math.h>

void convert_to_altitude_ft(float* pressure_mb, float *altitude_ft) {
  *altitude_ft = (1-pow(*pressure_mb/1013.25,0.190284))*145366.45;
}

void convert_to_altitude_ft_dp(double* pressure_mb, double *altitude_ft) {
  *altitude_ft = (1-pow(*pressure_mb/1013.25,0.190284))*145366.45;
}

void feet_to_meters(float *ft, float *m) {
  // =D18/3.280839895
  *m = *ft/3.280839895;
}

void feet_to_meters_dp(double *ft, double *m) {
  // =D18/3.280839895
  *m = *ft/3.280839895;
}

