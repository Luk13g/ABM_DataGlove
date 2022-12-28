#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "main.h"
#include "hw.h"
#include "app.h"
#include "nrf24.h"
#include "crc16.h"

extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim2;

#define HW_TIMER_US_PORT     htim2

#define HW_NRF24_SPI_PORT    hspi2
#define HW_NRF24_CE_PORT     RF_CE_GPIO_Port
#define HW_NRF24_CE_PIN      RF_CE_Pin
#define HW_NRF24_CSN_PORT    RF_CSN_GPIO_Port
#define HW_NRF24_CSN_PIN     RF_CSN_Pin
#define HW_NRF24_IRQ_PORT    RF_IRQ_GPIO_Port
#define HW_NRF24_IRQ_PIN     RF_IRQ_Pin
#define HW_NRF24_IRQ_NUMBER  RF_IRQ_EXTI_IRQn


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == HW_NRF24_IRQ_PIN)
	{
		nRF24_Interrupt();
	}
	else if(GPIO_Pin == SW_INT_Pin)
	{
		app_key_pressed();
	}
}

void hw_nrf24_ce_low(void)
{
	HAL_GPIO_WritePin(HW_NRF24_CE_PORT, HW_NRF24_CE_PIN, GPIO_PIN_RESET);
}

void hw_nrf24_ce_high(void)
{
	HAL_GPIO_WritePin(HW_NRF24_CE_PORT, HW_NRF24_CE_PIN, GPIO_PIN_SET);
}

// CSN (chip select negative) pin (PB12)
void hw_nrf24_csn_low(void)
{
	HAL_GPIO_WritePin(HW_NRF24_CSN_PORT, HW_NRF24_CSN_PIN, GPIO_PIN_RESET);
}

void hw_nrf24_csn_high(void)
{
	HAL_GPIO_WritePin(HW_NRF24_CSN_PORT, HW_NRF24_CSN_PIN, GPIO_PIN_SET);
}

void hw_nrf24_interrupt_set_state(bool state)
{
	if(state)
		HAL_NVIC_EnableIRQ(HW_NRF24_IRQ_NUMBER);
	else
		HAL_NVIC_DisableIRQ(HW_NRF24_IRQ_NUMBER);
}

void hw_nrf24_set_channel(uint8_t channel)
{
	hw_nrf24_ce_low();
	nRF24_SetRFChannel(HW_NRF23_CHANNEL_CALC(channel));
}

void hw_nrf24_init(void)
{
	hw_nrf24_interrupt_set_state(false);
	hw_nrf24_csn_high();
	hw_nrf24_ce_low();
	nRF24_Init();
}

// Low level SPI transmit/receive function (hardware depended)
// input:
//   data - value to transmit via SPI
// return: value received from SPI
uint8_t hw_nrf24_spi_rw(uint8_t tx_data)
{
	uint8_t rx_data;
	if(HAL_SPI_TransmitReceive(&HW_NRF24_SPI_PORT,&tx_data,&rx_data,1,100) == HAL_OK)
		return rx_data;
	else
		return 0;
}

uint32_t hw_timer_get_tick_ms(void)
{
	return HAL_GetTick();
}

uint32_t hw_timer_get_tick_us(void)
{
	return __HAL_TIM_GET_COUNTER(&HW_TIMER_US_PORT);
}

void hw_timer_init_us(void)
{
	HAL_TIM_Base_Start(&HW_TIMER_US_PORT);
}

uint32_t hw_timer_elapsed_us(uint32_t start)
{
	uint32_t elapsed;
	uint32_t now = hw_timer_get_tick_us();

	if(now < start)
		elapsed = (HAL_MAX_DELAY - start) + now + 1;
	else
		elapsed = now - start;

	return elapsed;
}

uint32_t hw_timer_elapsed_ms(uint32_t start)
{
	uint32_t elapsed;
	uint32_t now = hw_timer_get_tick_ms();

	if(now < start)
		elapsed = (HAL_MAX_DELAY - start) + now + 1;
	else
		elapsed = now - start;

	return elapsed;
}

void hw_timer_delay_us(uint32_t delay)
{
	  uint32_t start = hw_timer_get_tick_us();
	  uint32_t elapsed;
	  uint32_t now;

	  do
	  {
		  now = hw_timer_get_tick_us();

		  if(now < start)
			  elapsed = (HAL_MAX_DELAY - start) + now + 1;
		  else
			  elapsed = now - start;
	  }
	  while(elapsed < delay);
}

void hw_user_led_set(bool state)
{
	//GPIO_PinState pin_state = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	//HAL_GPIO_WritePin(USER_LED_GPIO_Port,USER_LED_Pin,pin_state);
}

void hw_user_led_toggle(void)
{
	//HAL_GPIO_TogglePin(USER_LED_GPIO_Port,USER_LED_Pin);
}

//##############################################################################################//
//Serial communication configuration

static void hw_uart_tx_byte(uint8_t c)
{
	USART_TypeDef *h = huart1.Instance;

	// garante que o shift register esta vazio
	while(!(h->SR & UART_FLAG_TXE))
	{}

	// coloca o valor no shift register
	h->DR = c;
}


void hw_uart_tx(uint8_t *buffer, uint32_t size)
{
	for(size_t pos = 0 ; pos < size ; pos++)
		hw_uart_tx_byte(buffer[pos]);
}

void hw_sleep(void)
{
	__WFI();
}

void hw_init(void)
{
	hw_timer_init_us();
	hw_user_led_set(false);
}

