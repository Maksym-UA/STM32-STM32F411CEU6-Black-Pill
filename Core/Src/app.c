/*
 * app.c
 *
 *  Created on: Apr 19, 2026
 *      Author: maksym
 */

#include "app.h"
#include <stdbool.h>

// --- Hardware Handles ---
// This tells the compiler: "huart1 exists in another file (main.c)"
extern UART_HandleTypeDef huart1;

// --- Private Variables ---
static uint8_t rx_byte;          // Buffer for 1 char
static bool is_blinking = false; // Toggle state
static uint32_t last_blink = 0;  // Blink timer
static uint32_t last_btn_press = 0; // Button debounce timer

// --- Interrupt Callback ---
// This function overrides the "Weak" definition in the HAL library.
// It is called automatically when UART receives data.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    if (rx_byte == 'T')
    {
      is_blinking = !is_blinking;
    }
    // Restart Listening
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
  }
}


void setup(void) {
	// 1. Initialize LED (Active Low -> SET = OFF)
	  // Using standard Black Pill Pin (PC13)
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	  // 2. Start UART Interrupt Listening
	  HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

void loop(void) {
	// 1. Handle Button Press (Send 'T')
	  // Black Pill PA0 is Active Low (GND when pressed)
	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
	  {
	    // Debounce: Ensure 250ms passed since last press
	    if (HAL_GetTick() - last_btn_press > 250)
	    {
	      uint8_t cmd = 'T';
	      HAL_UART_Transmit(&huart1, &cmd, 1, 10);
	      last_btn_press = HAL_GetTick(); // Reset timer
	    }
	  }

	  // 2. Handle Blinking
	  if (is_blinking)
	  {
	    if (HAL_GetTick() - last_blink > 500) // 500ms interval
	    {
	      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	      last_blink = HAL_GetTick();
	    }
	  }
	  else
	  {
	    // Ensure LED stays off when disabled
	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	  }

}
