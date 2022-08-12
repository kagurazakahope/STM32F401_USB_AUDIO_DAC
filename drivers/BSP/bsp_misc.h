#ifndef __BSP_MISC_H
#define __BSP_MISC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"

typedef enum {
 LED_4 = 0,
 LED_8,
 LED_9,
 LED_NONE,
 LED_MAX
} Led_TypeDef;

#define LED_NUMBER_PIN_COUNT 7

uint16_t led_number_pins[LED_NUMBER_PIN_COUNT];

int led_num_status[LED_MAX][LED_NUMBER_PIN_COUNT];


extern volatile uint32_t BtnPressed; // PA0

#define LED_GPIO_PORT                   GPIOB

#define LED_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()

#define ONBOARD_LED_PORT				GPIOC
#define ONBOARD_LED_PIN					GPIO_PIN_13
#define ONBOARD_LED_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()


void bsp_init(void);

void BSP_LED_Init(void);
void BSP_LED_DeInit(void);
void BSP_LED_Set(Led_TypeDef);
void BSP_PB_Init(void);
void BSP_OnboardLED_On(void);
void BSP_OnboardLED_Off(void);
void BSP_OnboardLED_Toggle(void);
uint32_t BSP_PB_GetState(void);

#ifdef __cplusplus
}
#endif

#endif 


