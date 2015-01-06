#include <stm32f30x.h>

#define MOISTURE 1
#define BATTERY  0

void f3d_a2d_init(void);
int f3d_read_adc(int device);
