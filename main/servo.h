#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define SERVO_CANAL   LEDC_CHANNEL_0
#define GPIO_SET_DIRECTION SERVO_GPIO

void servo_mover(int graus) {
    uint32_t duty = (500 + (graus * 2000) / 180) * 16383 / 20000;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, SERVO_CANAL, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, SERVO_CANAL);
}

void configuracao(int pino_servo){
    gpio_set_direction(pino_servo,GPIO_MODE_OUTPUT);

        ledc_timer_config_t timer = {
        .speed_mode      = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .timer_num       = LEDC_TIMER_0,
        .freq_hz         = 50,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t canal = {
        .gpio_num   = pino_servo,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = SERVO_CANAL,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
    };
    ledc_channel_config(&canal);

}

void configuracao_botao(int botao) {
    gpio_config_t io_conf = {0};
    io_conf.mode         = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << botao);
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pull_up_en   = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

void reset(int botao) {
    if (gpio_get_level(botao) == 1) {
        servo_mover(90);
        printf("resetado");
    }
}

#endif
