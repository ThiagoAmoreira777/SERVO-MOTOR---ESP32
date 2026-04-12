#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "servo.h"

#define POTENCIOMETRO ADC1_CHANNEL_6 // GPIO 34
#define LED_AZUL      25
#define LED_VERDE     26
#define LED_VERMELHO  27
#define BOTAO         21

void inicializacao_dos_leds(void) {
    gpio_set_direction(LED_AZUL,     GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_VERDE,    GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_VERMELHO, GPIO_MODE_OUTPUT);
}

void leds_atualizar(int graus) {
    // Apaga todos primeiro
    gpio_set_level(LED_AZUL,     0);
    gpio_set_level(LED_VERDE,    0);
    gpio_set_level(LED_VERMELHO, 0);

    if (graus <= 60) {
        gpio_set_level(LED_AZUL, 1);
    } else if (graus <= 120) {
        gpio_set_level(LED_VERMELHO, 1);
    } else {
        gpio_set_level(LED_VERDE, 1);
    }
}

void app_main(void) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(POTENCIOMETRO, ADC_ATTEN_DB_11);

    configuracao(18);
    configuracao_botao(21);
    inicializacao_dos_leds();

    static int ultimo_graus = 0; 

bool aguardando_reset = false; 

while (1) {
    int leitura = 0;
    for (int i = 0; i < 10; i++) {
        leitura += adc1_get_raw(POTENCIOMETRO);
        vTaskDelay(pdMS_TO_TICKS(2));
    }
    leitura /= 10;
    int graus = (leitura * 180) / 4095;

    // Botão pressionado: vai para 90° e trava
    if (gpio_get_level(BOTAO) == 1) {
        servo_mover(90);
        aguardando_reset = true;
    }

    // Só volta a seguir o potenciômetro quando ele estiver perto de 90°
    if (aguardando_reset) {
        if (abs(graus - 90) <= 5) {  // tolerância de 5°
            aguardando_reset = false;
        }
    } else {
        servo_mover(graus);
    }

    leds_atualizar(graus);
    vTaskDelay(pdMS_TO_TICKS(20));
}
}