/* HEADER */

void f3d_sdcard_init(void) {

}

static void xmit_mmc (const BYTE* buff, UINT bc) {
    // buff: pointer to data buffer to send
    // bc: byte count of data in the tranmit buffer
    int i;
    spiReadWrite(SPI2, 0, (uint8_t *)buff, bc, SPI_SLOW);
}

static void rcvr_mmc (BYTE *buff, UINT bc) {
    // buff: pointer to the receive buffer
    // bc: byte count of data in the receive buffer
    int i;
    spiReadWrite(SPI2, buff, 0, bc, SPI_SLOW);
}

