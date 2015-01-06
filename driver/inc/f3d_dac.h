#include <stm32f30x.h>

#define DAC_DHR12R2_ADDRESS  0x40007414
#define DAC_DHR8R1_ADDRESS   0x40007410

const uint16_t Sine12bit[32] = {
    2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
    3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
    599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
const uint8_t Escalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};

void f3d_dac_channel_setup(void);
void f3d_dac_init(void);
