#include "button_thread.h"
#include "rs_gpio_debounce/rs_gpio_debounce.h"
#include <stdio.h>

extern TaskHandle_t rgb_thread;

extern gpio_debounce_ctrl_t g_gpio_debounce_ctrl;
extern gpio_debounce_cfg_t  g_gpio_debounce_cfg;

void button_cb (const gpio_debounce_callback_args_t * p_args) {
    if (p_args->event == GPIO_DEBOUNCE_EVENT_PRESSED)
    {
        xTaskNotifyGive(rgb_thread);
    };
}

/* Button Thread entry function */
/* pvParameters contains TaskHandle_t */
void button_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    rs_gpio_debounce_Open(&g_gpio_debounce_ctrl, &g_gpio_debounce_cfg);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, (5 / portTICK_PERIOD_MS));
        rs_gpio_debounce_Update(&g_gpio_debounce_ctrl);
    }

}
