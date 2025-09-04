#include "Spi.h"
#include "BC3603_CMD_REG.h"

static const char *device = "/dev/spidev0.0";
static uint8_t mode = SPI_MODE_0;
static uint32_t speed = 100000;
static int fd;

int SpiInit() 
{
    fd = open(device, O_RDWR);
    if (fd < 0) 
    {
        perror("can't open device");
        return -1;
    }
    printf("SPI opened successfully\n");

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) 
    {
        perror("can't set spi mode");
        return -1;
    }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) 
    {
        perror("can't set max speed");
        return -1;
    }
    return 0;
}

int SpiTransfer(uint8_t *tx, uint8_t *rx, int len) 
{
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = len,
        .speed_hz = speed,
        .bits_per_word = 8,
    };
    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
}

void SpiClose(void)
{
    close(fd);
}
    
