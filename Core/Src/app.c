// Core/Src/app.c
#include "app.h"
#include "stm32f4xx.h"

// Adjust as needed for your hardware:
#define LED_PIN         13
#define BUTTON_PIN      0

const uint32_t blink_rates[] = {1000, 250, 100};
const uint8_t num_rates = sizeof(blink_rates)/sizeof(blink_rates[0]);

static void delay_ms(uint32_t ms) {
    // Blocking delay, assuming 100MHz (Black Pill default)
    volatile uint32_t n;
    while (ms--) {
        n = 25000; // adjust for your system core clock if needed
        while (n--) __NOP();
    }
}

void setup(void) {
    // Enable GPIOC and GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;

    // Configure PC13 as output (LED)
    GPIOC->MODER &= ~(3 << (LED_PIN * 2));
    GPIOC->MODER |=  (1 << (LED_PIN * 2));   // Output mode

    GPIOC->OTYPER &= ~(1 << LED_PIN);        // Push-pull
    GPIOC->OSPEEDR |= (3 << (LED_PIN * 2));  // High speed (optional)
    GPIOC->PUPDR  &= ~(3 << (LED_PIN * 2));  // No pull

    // Set initial LED state to OFF (Black Pill: 1 = off, 0 = on)
    GPIOC->ODR |= (1 << LED_PIN);

    // Configure PA0 as input with Pull-Up (button)
    GPIOA->MODER &= ~(3 << (BUTTON_PIN * 2));               // Input
    GPIOA->PUPDR &= ~(3 << (BUTTON_PIN * 2));               // Clear
    GPIOA->PUPDR |=  (1 << (BUTTON_PIN * 2));               // Pull-up
}

void loop(void) {
    static int prev_btn = 1;
    static uint8_t rate_idx = 0;
    static uint32_t last_toggle = 0;
    static int led_is_on = 0;

    // Button state (pressed=0 because pull-up)
    int btn = (GPIOA->IDR & (1 << BUTTON_PIN)) ? 1 : 0;

    if (prev_btn && !btn) {
        // Button pressed (falling edge)
        rate_idx = (rate_idx + 1) % num_rates;
        delay_ms(100); // debounce
    }
    prev_btn = btn;

    // Get a timestamp (use SysTick or HAL_GetTick for real precision, here is just delay for demo)
    static uint32_t now_tick = 0;
    uint32_t interval = blink_rates[rate_idx];

    if (++now_tick >= interval) {
        now_tick = 0;

        // Toggle LED: PC13 (active low)
        if (led_is_on) {
            GPIOC->ODR |= (1 << LED_PIN);  // OFF
            led_is_on = 0;
        } else {
            GPIOC->ODR &= ~(1 << LED_PIN); // ON
            led_is_on = 1;
        }
    }

    delay_ms(1); // ensures a "tick" per ms
}
