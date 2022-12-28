#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "nrf24.h"
#include "nrf24_app.h"
#include "stm32f1xx_hal.h"
#include "hw.h"
#include "utility.h"
#include "MPU6050.h"
#include "s_luxflex.h"

extern TIM_HandleTypeDef htim4;
extern I2C_HandleTypeDef hi2c1;
MPU6050_t MPU6050;

//Sensores de linha e bateria
volatile extern uint32_t adc_data_avg[ADC_NUM_CHANNELS];

static uint8_t buffer_test[32];

typedef enum sensor_data_e
{
	DATA_ACEL_X = 0,
	DATA_ACEL_Y = 1,
	DATA_ACEL_Z = 2,
	DATA_KEY = 3,
	DATA_LUX = 4,
	DATA_POT = 5,
} sensor_data_t;

volatile bool app_started = false;
volatile uint32_t last_recv_ms = 0;

#define APP_UART_BUFFER_MAX   128
volatile float cont_time = 0;

//#define SERIAL_DEBUG_MPU6050	    1
#define SERIAL_DEBUG_ANALOG_SENSORS	1

nrf24_config_t nrf24_config =
{
	.channel = 17,
	.data_rate = nRF24_DR_2Mbps,
	.rdelay = nRF24_ARD_500us,
	.rcount = 3,
	.tx_power = nRF24_TXPWR_0dBm,
	.network_address = { 'T', 'E', 'S', '\0', '\0' },
	.address_size = 3,
};

bool app_is_started(void)
{
	return app_started;
}

void HAL_SYSTICK_Callback(void)
{
}

static void app_rx_cbk_func(nRF24_pipe_number_t pipe, uint8_t *buffer, uint8_t length, nrf24_tx_result_t status)
{
	if(status == NRF24_APP_OK)
	{
		last_recv_ms = hw_timer_get_tick_ms();

		if(buffer[0] == '1')
		{
			hw_user_led_toggle();
		}
	}
}

static void app_tx_cbk_func(nRF24_pipe_number_t pipe, uint8_t *buffer, uint8_t length, nrf24_tx_result_t status)
{
	if(status == NRF24_APP_OK)
	{
	}
}

void app_reset_radio(uint8_t rf_channel)
{
    nrf24_app_init(&nrf24_config,app_tx_cbk_func,app_rx_cbk_func);
    hw_nrf24_set_channel(rf_channel);
}

void app_key_pressed(void)
{
	static uint8_t buffer[32];
	buffer[0] = '1';

	nrf24_app_tx_data(buffer,32);
}

void app_init(void)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,SET);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,SET);
	HAL_TIM_Base_Start_IT(&htim4);
	adc_calibration();
	hw_init();
    nrf24_app_init(&nrf24_config,app_tx_cbk_func,app_rx_cbk_func);

    app_started = true;
    while (MPU6050_Init(&hi2c1) == 1);// inicializa MPU6050

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim4)//le o sensor a cada 200 ms
	{

		MPU6050_Read_Accel(&hi2c1, &MPU6050);//mpu consome 8756 bytes de flash e 20 de ram e radio 7156 bytes de flash

#ifdef SERIAL_DEBUG_MPU6050
		static uint8_t tx_buffer[APP_UART_BUFFER_MAX];
		uint32_t tx_size;
		cont_time += 0.2;
		//tx_size = snprintf((char *)tx_buffer,APP_UART_BUFFER_MAX-1,"%d;%d;%d;%f\n", (short int)MPU6050.Accel_X_RAW, (short int)MPU6050.Accel_Y_RAW, (short int)MPU6050.Accel_Z_RAW, (float)cont_time);
		tx_size = snprintf((char *)tx_buffer,APP_UART_BUFFER_MAX-1,"Accel_X: %d;  Accel_Y: %d;  Accel_Z: %d; %f \n", (short int)MPU6050.Accel_X_RAW, (short int)MPU6050.Accel_Y_RAW, (short int)MPU6050.Accel_Z_RAW, (float)cont_time);
		hw_uart_tx(tx_buffer,tx_size);
#endif
	}
}

void app_loop(void)
{

	while(1)
	{

	    uint16_t adc1_val_ch1, adc1_val_ch4;


	    buffer_test[DATA_ACEL_X] = map(MPU6050.Accel_X_RAW,-17000,17000, 0 ,255);
	    buffer_test[DATA_ACEL_Y] = map(MPU6050.Accel_Y_RAW,-17000,17000, 0 ,255);
	    buffer_test[DATA_ACEL_Z] = map(MPU6050.Accel_Z_RAW,-17000,17000, 0 ,255);
	    buffer_test[DATA_KEY] = (HAL_GPIO_ReadPin(KEY_SELECT_GPIO_Port, KEY_SELECT_Pin)==GPIO_PIN_SET) ? (true) : (false);


	    if(buffer_test[DATA_KEY])
	    	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,SET);
	    else
	    	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,RESET);


	    hw_adc_start();
	    adc_read_channels(&adc1_val_ch1);
	    buffer_test[DATA_LUX] = map(adc1_val_ch1, 0, 4096, 0, 255);//S1_LUXFLEX
	    adc_read_channels(&adc1_val_ch4);

		// map(adc1_val_ch0, 0, 4096, 0, 255);//S2_LUXFLEX

		buffer_test[DATA_POT] = map(adc1_val_ch4, 0, 4096, 0, 255);//S_POT // 0 a 50

#ifdef SERIAL_DEBUG_ANALOG_SENSORS
		static uint8_t tx_buffer[APP_UART_BUFFER_MAX];
		uint32_t tx_size;
		tx_size = snprintf((char *)tx_buffer,APP_UART_BUFFER_MAX-1,"S1_LUXFLEX = %d /S_POT = %d\n", (unsigned short)buffer_test[DATA_LUX], (unsigned short)adc1_val_ch4);
		hw_uart_tx(tx_buffer,tx_size);
#endif

	    hw_adc_stop();

	    nrf24_app_tx_data(buffer_test,32);

	    HAL_Delay(50);
	}
}
