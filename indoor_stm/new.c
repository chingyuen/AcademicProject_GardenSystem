/* nrf24l01base_initialize_debug(false, 1, false);  // Setup Node at transmitter, Standard Shockburst */
nrf24l01base_initialize_debug(false, 32, true);     // Enhanced Shockburst, Auto Ack turned on
nrf24l01base_clear_flush();

char data[32];
char index;

for (index=0;index<32;index++) {
				data[index] = 'a'+index;
}
printf("Node Data: ");
for (index=0;index<32;index++) {
				printf("%c",data[index]);
}
printf("\n");
printf("Node: transmit character %c\n",data);
nrf24l01base_write_tx_payload(data, 32, true);      // nordic writes a character
while(!(nrf24l01base_irq_pin_active() && (nrf24l01base_irq_tx_ds_active() || nrf24l01base_irq_max_rt_active()))); // wait until it is gone

// Node Listen Mode if the maximum retry limit was not hit
if (!nrf24l01base_irq_max_rt_active()) {
				nrf24l01base_irq_clear_all();
				nrf24l01base_set_as_rx(true);
				printf("true\n");
}
else {
				nrf24l01base_flush_tx(); //get the unsent character out of the TX FIFO
				nrf24l01base_irq_clear_all(); //clear all interrupts
				printf("Node: Failed to send %c\n",data);
}

// Node reception

printf("node reception\n");
char noderx[32];
while(!(nrf24l01base_irq_pin_active() && nrf24l01base_irq_rx_dr_active()));
nrf24l01base_read_rx_payload(noderx, 32);

printf("Node Data: ");
for (index=0;index<32;index++) {
				printf("%c",noderx[index]);
}
printf("\n");

printf("Node: Receive character %c\n",noderx);
nrf24l01base_irq_clear_all();
f3d_delay_uS(130);
nrf24l01base_set_as_tx();

printf("#### End Wifi Test ####\n");

