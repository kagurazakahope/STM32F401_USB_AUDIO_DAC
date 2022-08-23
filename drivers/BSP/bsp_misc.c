#include "main.h"
#include "bsp_misc.h"

void Error_Handler(void);

volatile uint32_t BtnPressed = 0;

uint16_t led_number_pins[LED_NUMBER_PIN_COUNT] = {
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
};

int led_num_status[LED_MAX][LED_NUMBER_PIN_COUNT] = {
    [LED_4] = {0,1,1,0,0,1,1},
    [LED_8] = {1,1,1,1,1,1,1},
    [LED_9] = {1,1,1,1,0,1,1},
    [LED_NONE] = {0,0,0,0,0,0,0},
};

void bsp_init(void) {
	BSP_PB_Init();
	BSP_LED_Init();
	}

void BSP_LED_Init(void) {
	GPIO_InitTypeDef  gpio_init_structure = {0};
	for (int i = 0; i < LED_NUMBER_PIN_COUNT; ++i)
		gpio_init_structure.Pin |= led_number_pins[i];
	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_LOW;

	LED_GPIO_CLK_ENABLE();
	HAL_GPIO_Init(LED_GPIO_PORT, &gpio_init_structure);

	for (int i = 0; i < LED_NUMBER_PIN_COUNT; ++i)
		HAL_GPIO_WritePin(LED_GPIO_PORT, led_number_pins[i], GPIO_PIN_SET); // LEDs are active low

	gpio_init_structure.Pin   = ONBOARD_LED_PIN;
	ONBOARD_LED_GPIO_CLK_ENABLE();
	HAL_GPIO_Init(ONBOARD_LED_PORT, &gpio_init_structure);
	HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_SET); // onboard led is active low
}


void BSP_LED_DeInit(void){
	for (int i = 0; i < LED_NUMBER_PIN_COUNT; ++i)
		HAL_GPIO_WritePin(LED_GPIO_PORT, led_number_pins[i], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_RESET);

	GPIO_InitTypeDef  gpio_init_structure = {0};
	for (int i = 0; i < LED_NUMBER_PIN_COUNT; ++i)
		gpio_init_structure.Pin |= led_number_pins[i];
	HAL_GPIO_DeInit(LED_GPIO_PORT, gpio_init_structure.Pin);

	gpio_init_structure.Pin   = ONBOARD_LED_PIN;
	HAL_GPIO_DeInit(ONBOARD_LED_PORT, gpio_init_structure.Pin);
	}


void BSP_OnboardLED_On(void) {
    HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_RESET);
	}

void BSP_OnboardLED_Off(void) {
    HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_SET);
	}

void BSP_OnboardLED_Toggle(void) {
    HAL_GPIO_TogglePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN);
	}

void BSP_LED_Set(Led_TypeDef Led) {
	for (int i = 0; i < LED_NUMBER_PIN_COUNT; ++i)
		// Since STM32F4x1 has sum(Iio) +- 120 mA, we need MOSFET to drive the LED,
		//  so with a common anode LED it's easier if active-high
		HAL_GPIO_WritePin(LED_GPIO_PORT, led_number_pins[i], led_num_status[Led][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void BSP_PB_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA0  (KEY button on board) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}



uint32_t BSP_PB_GetState(void) {
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}


void HAL_GPIO_EXTI_Callback(uint16_t gpioPin) {
	if (gpioPin == GPIO_PIN_0) {
		BtnPressed = 1;
		}
	}

