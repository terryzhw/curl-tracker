#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"


#define LED_GPIO 3
#define BUTTON_GPIO 4

#define PWM_MAX ((1<<13)-1)

// void blinkLED(void *pvParameters) {
//     gpio_config_t led_config = {
//         .pin_bit_mask = (1ULL << LED_GPIO),
//         .mode = GPIO_MODE_OUTPUT,
//     };
//     gpio_config(&led_config);


//     for (;;) {
//         gpio_set_level(LED_GPIO,1);
//         vTaskDelay(pdMS_TO_TICKS(500));
//         gpio_set_level(LED_GPIO, 0);
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

void pollButton(void *pvParameters) {
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&button_config);

    for(;;) {
        if (gpio_get_level(BUTTON_GPIO) == 0) {
            printf("button pressed\n");
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void breatheLED(void *pvParamters) {

    //set up pwm

    ledc_timer_config_t pwm_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE, // S2, S3, C3, C6, H2 only have LEDC_LOW_SPEED_MODE
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_13_BIT, //0-8191
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_channel_config_t pwm_channel ={
        .gpio_num = LED_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 0,
        .hpoint = 0,
    };

    ledc_timer_config(&pwm_timer);
    ledc_channel_config(&pwm_channel);

    for (;;) {
        for (int i=0; i<=PWM_MAX; i += 64) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        for (int i=PWM_MAX; i>=0; i -= 64) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

void app_main(void)
{


    // xTaskCreate(blinkLED, "BlinkLED", 2048, NULL, 1, NULL);
    xTaskCreate(breatheLED, "BreatheLED", 2048, NULL, 1, NULL);
    xTaskCreate(pollButton, "pollButton", 2048, NULL, 1, NULL);


}