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
#include <pthread.h>
#include <stdlib.h>

#include "My_lib/Spi.h"
#include "My_lib/BC3603.h"
#include "My_lib/BC3603_CMD_REG.h"
#include "My_lib/RF_API.h"
#include "My_lib/SYS.h"

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 25
#define LOG_QUEUE_SIZE 128
#define LOG_BATCH_FLUSH 10

extern _BC3603_device_ BC3603_T;
static volatile sig_atomic_t running = 1;
volatile u8 Ngat = 0;

// ---------------- Queue log ----------------
typedef struct {
    char timestr[64];
    size_t len;
    unsigned char data[256];
} log_entry_t;

typedef struct {
    log_entry_t entries[LOG_QUEUE_SIZE];
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} log_queue_t;

log_queue_t log_queue = {
    .head = 0,
    .tail = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER
};

// Push entry vào queue (thread-safe)
void log_queue_push(const log_entry_t *entry)
{
    pthread_mutex_lock(&log_queue.mutex);
    int next = (log_queue.head + 1) % LOG_QUEUE_SIZE;
    if (next == log_queue.tail) 
    {
        // Queue full, bỏ bản ghi cũ
        log_queue.tail = (log_queue.tail + 1) % LOG_QUEUE_SIZE;
    }
    log_queue.entries[log_queue.head] = *entry;
    log_queue.head = next;
    pthread_cond_signal(&log_queue.cond);
    pthread_mutex_unlock(&log_queue.mutex);
}

// Pop entry từ queue
int log_queue_pop(log_entry_t *entry)
{
    pthread_mutex_lock(&log_queue.mutex);
    while (log_queue.head == log_queue.tail && running) 
    {
        pthread_cond_wait(&log_queue.cond, &log_queue.mutex);
    }
    if (log_queue.head == log_queue.tail) 
    {
        pthread_mutex_unlock(&log_queue.mutex);
        return -1;
    }
    *entry = log_queue.entries[log_queue.tail];
    log_queue.tail = (log_queue.tail + 1) % LOG_QUEUE_SIZE;
    pthread_mutex_unlock(&log_queue.mutex);
    return 0;
}

// Thread ghi file log
void* file_writer_thread(void* arg)
{
    log_entry_t entry;

    while (running || log_queue.head != log_queue.tail) 
    {
        if (log_queue_pop(&entry) == 0)   // chỉ chạy khi có dữ liệu
        {
            FILE *f = fopen("received_log.txt", "a");  // mở file khi cần
            if (!f) continue;

            // ghi log
            fprintf(f, "[%s] ", entry.timestr);
            for (size_t i = 0; i < entry.len; i++) 
            {
                unsigned char c = entry.data[i];
                fputc((c >= 32 && c <= 126) ? c : '.', f);
            }
            fputc('\n', f);

            fclose(f);   // đóng ngay sau khi ghi
        }
    }

    return NULL;
}

// ---------------- GPIO ----------------
void cleanup_GPIO(struct gpiod_line *line);
struct gpiod_line* setup_GPIO(const char *chip_name, unsigned int line_num);
void sigint_handler(int sig);

// ---------------- Main ----------------
int main(void)
{
    int ret = 0;
    struct gpiod_line *line = NULL;
    struct gpiod_line_event event;
    struct timespec timeout;
    pthread_t writer_thread;

    // Ctrl+C
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

    // Start log thread
    pthread_create(&writer_thread, NULL, file_writer_thread, NULL);

    timeout.tv_sec = 0;
    timeout.tv_nsec = 100 * 1000000; // 100 ms

    while (running) 
    {
        // GPIO interrupt
        int ev_ready = gpiod_line_event_wait(line, &timeout);
        if (ev_ready == 1) {
            if (gpiod_line_event_read(line, &event) == 0) 
            {
                if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) 
                {
                    Ngat = 1;
                }
            }
        }

        // Process RF
        ATR_WOR_Process(&BC3603_T);

        if (BC3603_T.rx_irq_f) 
        {
            size_t len = BC3603_T.rec_data_len;

            // Print terminal
            printf("Received: ");
            for (size_t i = 0; i < len; i++) 
            {
                unsigned char c = BC3603_T.rx_payload_buffer[i];
                if (c == 0) break;
                if (c >= 32 && c <= 126)
                    printf("%c", c);
            }
            printf("\n");

            // Push vào queue log
            time_t now = time(NULL);
            char timestr[64];
            strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now));

            log_entry_t entry;
            strncpy(entry.timestr, timestr, sizeof(entry.timestr));
            entry.len = len > 256 ? 256 : len;
            memcpy(entry.data, BC3603_T.rx_payload_buffer, entry.len);

            log_queue_push(&entry);

            // Reset RF module ngay lập tức
            BC3603_T.step = 0;
            BC3603_T.rx_irq_f = 0;
            BC3603_T.mode = RF_RX;
        }

        if (Ngat) Ngat = 0;
        usleep(100); // 0.1 ms sleep, tăng tốc loop
    }

cleanup:
    SpiClose();
    cleanup_GPIO(line);

    // Stop log thread
    pthread_mutex_lock(&log_queue.mutex);
    pthread_cond_signal(&log_queue.cond);
    pthread_mutex_unlock(&log_queue.mutex);
    pthread_join(writer_thread, NULL);

    printf("Clean exit.\n");
    return ret;
}

// ---------------- GPIO functions ----------------
void sigint_handler(int sig)
{
    (void)sig;
    running = 0;
    printf("\nExiting...\n");
}

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

void cleanup_GPIO(struct gpiod_line *line)
{
    if (line) 
    {
        struct gpiod_chip *chip = gpiod_line_get_chip(line);
        gpiod_line_release(line);
        gpiod_chip_close(chip);
    }
}
