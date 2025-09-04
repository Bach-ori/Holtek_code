#ifndef SPI_H
#define SPI_H

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int SpiInit(void);
int SpiTransfer(uint8_t *tx, uint8_t *rx, int len);
void SpiClose(void);

#endif