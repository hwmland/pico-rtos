#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "pico/stdlib.h"

// Nomen omens. Fully uses CPU for given amount of time.
static void _busy_wait_ms(uint32_t ms)
{
    uint64_t end = time_us_64() + (uint64_t)ms * (uint64_t)1000;
    while (time_us_64() < end)
    {
        // busy wait
    }
}

// Blink the LED on the board
// By default on RPi Pico it is pin 25
void led_task(void *arg)
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true)
    {
        gpio_put(LED_PIN, 1);
        vTaskDelay(100);
        gpio_put(LED_PIN, 0);
        vTaskDelay(100);
    }
}

// Argument is number of milliseconds to delay/run on MCU
// Function busy waits for that amount of time and than sleeps for that amount of time
void cpurunner_task(void *arg)
{
    int delay = (int)arg;
    while (true)
    {
        _busy_wait_ms(delay);
        vTaskDelay(delay);
    }
}

// Queue for producer-consumer pattern
// queue element is 64bit number, where first 32bits is producer id and second 32bits is element id
static QueueHandle_t producerConsumerQueue;

// Argument is number of elements to produce in one batch (16bits) + producer id (16bits)
// Function produces that many elements and put them into queue.
// Production is 200ms MCU-bound operation.
// Than task sleeps for 'batch size' seconds.
void producer_task(void *arg)
{
    uint param = (uint)arg;
    uint batchSize = param >> 16;
    uint producerId = param & 0xFFFF;
    int i = 0;
    uint64_t queueElement;

    while (true)
    {
        for (int element = 0; element < batchSize; element++)
        {
            queueElement = ((uint64_t)producerId << 32) + i;
            _busy_wait_ms(200);
            printf("Producer %d: %d\n", producerId, i);
            xQueueSend(producerConsumerQueue, &queueElement, portMAX_DELAY);
            i++;
        }

        vTaskDelay(batchSize * 1000);
    }
}

// Function consumes elements from queue. Each element processing is 1000ms MCU-bound operation.
void consumer_task(void *arg)
{
    while (true)
    {
        uint64_t queueElement;
        uint producerId;
        uint i;
        xQueueReceive(producerConsumerQueue, &queueElement, portMAX_DELAY);
        producerId = queueElement >> 32;
        i = queueElement & 0xFFFFFFFF;
        _busy_wait_ms(1000);
        printf("Consumer (%d: %d)\n", producerId, i);
    }
}
int main()
{
    stdio_init_all();

    xTaskCreate(led_task, "LED_Task", 256, NULL, 10, NULL);
    xTaskCreate(cpurunner_task, "Busy_1s", 256, (void *)1000, 5, NULL);
    xTaskCreate(cpurunner_task, "Busy_10s", 256, (void *)10000, 5, NULL);

    producerConsumerQueue = xQueueCreate(10, sizeof(uint64_t));
    xTaskCreate(producer_task, "Producer_A", 256, (void *)((5 << 16) + 1), 8, NULL);
    xTaskCreate(producer_task, "Producer_B", 256, (void *)((10 << 16) + 2), 8, NULL);
    xTaskCreate(consumer_task, "Consumer", 256, NULL, 15, NULL);

    vTaskStartScheduler();

    while (1)
    {
    };
}
