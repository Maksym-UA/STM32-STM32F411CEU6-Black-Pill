/*
 * app.c
 *
 *  Created on: Apr 19, 2026
 *      Author: maksym
 */

#include "app.h"

void setup(void) {
    // Спробуємо RESET як вимкнення. Якщо горить — замініть на SET.
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}



void loop(void) {
    static uint8_t prev_button_state = 1; // pull-up, idle high
    static uint8_t blinking = 0;
    static uint32_t last_blink_tick = 0;
    static GPIO_PinState led_state = GPIO_PIN_RESET;

    // Button state
    uint8_t button_state = HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin);

    // Toggle mode on button press (falling edge)
    if (prev_button_state == 1 && button_state == 0) {
        blinking = !blinking;
        HAL_Delay(100); // debounce (100ms)
    }
    prev_button_state = button_state;

    // Blinking logic
    if (blinking) {
        if (HAL_GetTick() - last_blink_tick >= 250) { // Blink interval (ms)
            led_state = (led_state == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, led_state);
            last_blink_tick = HAL_GetTick();
        }
    } else {
        // Make sure LED is off when not blinking
        if (led_state != GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
            led_state = GPIO_PIN_RESET;
        }
    }
}
