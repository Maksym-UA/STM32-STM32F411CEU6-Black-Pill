// Core/Src/app.c
#include "app.h"
#include "stm32f4xx.h" //MCU-specific register/maps for STM32F4 devices.

#define LED_PIN    13
#define BUTTON_PIN 0

const uint32_t blink_rates[] = {500, 100, 50}; // ms
const uint8_t num_rates = sizeof(blink_rates)/sizeof(blink_rates[0]);

static void delay_ms(uint32_t ms) {
    volatile uint32_t n;
    while (ms--) {
        n = 25000; // Adjust for actual clock speed (25000 = ~1ms on 100MHz F411)
        // intrinsic function provided by ARM CMSIS to the compiler;
        //it emits a single No Operation instruction
        while (n--) __NOP();
    }
}

void setup(void) {
    // Enable clocks for GPIOC and GPIOA (so registers are accessible)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;

    // PC13 (LED) Output Setup
    GPIOC->MODER &= ~(3 << (LED_PIN * 2));         // Clear mode bits
    GPIOC->MODER |=  (1 << (LED_PIN * 2));         // Set as output (01)
    GPIOC->OTYPER &= ~(1 << LED_PIN);              // Set as push-pull
    GPIOC->OSPEEDR |= (2 << (LED_PIN * 2));        // (Optional) High speed
    GPIOC->PUPDR &= ~(3 << (LED_PIN * 2));         // No pull-up/pull-down
    GPIOC->ODR |= (1 << LED_PIN);                  // Set pin HIGH (LED off, active-low LED)

    // PA0 (Button) Input + Pull-Up setup
    GPIOA->MODER &= ~(3 << (BUTTON_PIN * 2));      // Input mode (00)
    GPIOA->PUPDR &= ~(3 << (BUTTON_PIN * 2));      // Clear pull settings
    GPIOA->PUPDR |=  (1 << (BUTTON_PIN * 2));      // Enable pull-up (01)
}

void loop(void) {
    static int prev_btn = 1;
    static uint8_t rate_idx = 0;
    static int led_is_on = 0;
    static uint32_t now_tick = 0;

    // Read button (0 when pressed)
    int btn = (GPIOA->IDR & (1 << BUTTON_PIN)) ? 1 : 0;

    if (prev_btn && !btn) {
        rate_idx = (rate_idx + 1) % num_rates;
        delay_ms(100); // Debounce
    }
    prev_btn = btn;

    if (++now_tick >= blink_rates[rate_idx]) {
        now_tick = 0;
        // Toggle LED
        if (led_is_on) {
            GPIOC->ODR |= (1 << LED_PIN);   // LED off (active-low)
            led_is_on = 0;
        } else {
            GPIOC->ODR &= ~(1 << LED_PIN); // LED on
            led_is_on = 1;
        }
    }

    delay_ms(1);
}
