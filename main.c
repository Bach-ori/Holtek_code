#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <gpiod.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "My_lib/Spi.h"
#include "My_lib/BC3603.h"
#include "My_lib/BC3603_CMD_REG.h"
#include "My_lib/RF_API.h"
#include "My_lib/SYS.h"

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 25

extern _BC3603_device_ BC3603_T;
unsigned char *RX_data;
static volatile sig_atomic_t running = 1;
volatile u8 Ngat = 0;

void cleanup_GPIO(struct gpiod_line *line);
struct gpiod_line* setup_GPIO(const char *chip_name, unsigned int line_num);
void sigint_handler(int sig);

int main(void)
{
    int ret = 0;
    struct gpiod_line *line = NULL;
    struct gpiod_line_event event;
    struct timespec timeout;

    // Bắt Ctrl+C
    if (signal(SIGINT, sigint_handler) == SIG_ERR) 
    {
        perror("signal");
    }

    // Init RF
    RF_Init();
    RF_Parameter_loading();

    // Init GPIO
    line = setup_GPIO(GPIO_CHIP, GPIO_LINE);
    if (!line) 
    {
        ret = 1;
        goto cleanup;
    }

    // Mở file log append
    FILE *f = fopen("received_log.txt", "a");
    if (!f) {
        perror("fopen");
        f = NULL;
    }

    timeout.tv_sec = 0;
    timeout.tv_nsec = 100 * 1000000; // 100 ms

    while (running) 
    {
        int ev_ready = gpiod_line_event_wait(line, &timeout);
        if (ev_ready == 1) 
        {
            if (gpiod_line_event_read(line, &event) == 0) 
            {
                if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) 
                {
                    Ngat = 1;
                } 
            } 
        }

        ATR_WOR_Process(&BC3603_T);

        if (BC3603_T.rx_irq_f) 
        {
            size_t len = BC3603_T.rec_data_len;

            // In ra terminal
            printf("Received: ");
            for (size_t i = 0; i < len; i++) 
            {
                unsigned char c = BC3603_T.rx_payload_buffer[i];
                if (c == 0) break;
                if (c >= 32 && c <= 126)
                    printf("%c", c);
            }
            printf("\n");

            // Ghi log file với timestamp
            if (f) 
            {
                time_t now = time(NULL);
                char timestr[64];
                strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now));
                fprintf(f, "[%s] ", timestr);
                for (size_t i = 0; i < len; i++) 
                {
                    unsigned char c = BC3603_T.rx_payload_buffer[i];
                    if (c >= 32 && c <= 126)
                        fputc(c, f);
                    else
                        fputc('.', f);
                }
                fputc('\n', f);
                fflush(f); // đảm bảo ghi ngay
            }

            RX_data = BC3603_T.rx_payload_buffer;
            BC3603_T.step = 0;
            BC3603_T.rx_irq_f = 0;
            BC3603_T.mode = RF_RX;
        }

        if (Ngat) Ngat = 0;
        usleep(1000); // 1 ms
    }

cleanup:
    SpiClose();
    cleanup_GPIO(line);
    if (f) fclose(f);

    printf("Clean exit.\n");
    return ret;
}

void sigint_handler(int sig)
{
    (void)sig;
    running = 0;
    printf("\nExiting...\n");
}

// --- Hàm khởi tạo GPIO ---
struct gpiod_line* setup_GPIO(const char *chip_name, unsigned int line_num)
{
    struct gpiod_chip *chip = gpiod_chip_open(chip_name);
    if (!chip) 
    {
        perror("Open gpiochip failed");
        return NULL;
    }

    struct gpiod_line *line = gpiod_chip_get_line(chip, line_num);
    if (!line) 
    {
        perror("Get GPIO line failed");
        gpiod_chip_close(chip);
        return NULL;
    }

    if (gpiod_line_request_falling_edge_events(line, "my-gpio") < 0) 
    {
        perror("Request falling edge event failed");
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return NULL;
    }

    printf("GPIO%d interrupt setup done.\n", line_num);
    return line;
}

// --- Hàm cleanup GPIO ---
void cleanup_GPIO(struct gpiod_line *line)
{
    if (line) 
    {
        struct gpiod_chip *chip = gpiod_line_get_chip(line);
        gpiod_line_release(line);
        gpiod_chip_close(chip);
    }
}
