#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_RED 11
#define LED_YELLOW 12
#define LED_GREEN 13
#define BUTTON 5

uint32_t estado = 0;
bool timer_ativo = false; // Indica se o temporizador está rodando

// Callback do temporizador
int64_t led_callback(alarm_id_t id, void *user_data) {
    if (estado == 0) {
        gpio_put(LED_RED, 1);
        gpio_put(LED_YELLOW, 1);
        gpio_put(LED_GREEN, 1);
        estado = 1;
    } else if (estado == 1) {
        gpio_put(LED_RED, 0);  // Apaga o vermelho 
        estado = 2;
    } else if (estado == 2) {
        gpio_put(LED_YELLOW, 0);  // Apaga o amarelo
        estado = 3;
    } else if (estado == 3) {
        gpio_put(LED_GREEN, 0);  // Apaga o verde
        estado = 0;  // Reinicia o ciclo
        timer_ativo = false; // Desativa o temporizador
        return 0; // Para o temporizador
    }

    return 3000000; // Mantém o temporizador ativo (3s)
}

// Função para verificar o botão e iniciar o temporizador
void verificar_botao() {
    if (gpio_get(BUTTON) == 0 && !timer_ativo) { // Se o botão for pressionado e o timer estiver parado
        timer_ativo = true;
        add_alarm_in_ms(3000, led_callback, NULL, false);
    }
}

int main() {
    stdio_init_all();

    // Configuração dos LEDs
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, 0);

    gpio_init(LED_YELLOW);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_put(LED_YELLOW, 0);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0);

    // Configuração do botão
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    while (true) {
        verificar_botao(); // Verifica o botão
        sleep_ms(100); // Pequeno delay para evitar bouncing
    }
    return 0;
}
