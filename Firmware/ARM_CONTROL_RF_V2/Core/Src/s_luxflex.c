#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "s_luxflex.h"

extern ADC_HandleTypeDef hadc1;

//#define ADC_NUM_SAMPLES  15

//uint16_t adc_data[ADC_NUM_CHANNELS];
//volatile uint32_t adc_data_avg[ADC_NUM_CHANNELS];
//volatile uint32_t distance;

void adc_calibration(void)
{
	HAL_ADCEx_Calibration_Start(&hadc1);
}

void hw_adc_start(void)
{
	HAL_ADC_Start(&hadc1);
}

void hw_adc_stop(void)
{
	HAL_ADC_Stop(&hadc1);
}

bool adc_read_channels(uint16_t *adc_val_ch)
{
	HAL_StatusTypeDef adc_hal_status;

	while(1)
	{
			adc_hal_status = HAL_ADC_PollForConversion(&hadc1, 100);
			if (adc_hal_status == HAL_OK)
			{
			*adc_val_ch = (uint16_t) HAL_ADC_GetValue(&hadc1);

			return true;
			}

			else if (adc_hal_status != HAL_OK){
				return false;
			}
	}
}

/*void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	static uint32_t adc_data_sum[ADC_NUM_CHANNELS] = { 0 };
	static uint32_t num_conv = 0;
	uint32_t n;

	if(hadc == &hadc1)
	{
		for(n = 0 ; n < ADC_NUM_CHANNELS ; n++)
			adc_data_sum[n] += adc_data[n];

		if(num_conv++ > ADC_NUM_SAMPLES)
		{
			num_conv = 0;

			for(n = 0 ; n < ADC_NUM_CHANNELS ; n++)
			{
				adc_data_avg[n] = adc_data_sum[n] / ADC_NUM_SAMPLES;
				adc_data_sum[n] = 0;
			}
		}
	}
}*/

/*void adc_init(void)
{
	uint32_t n;

	for(n = 0 ; n < ADC_NUM_CHANNELS ; n++)
		adc_data_avg[n] = 0;

	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&adc_data[0],ADC_NUM_CHANNELS);
}*/

/* adc_data_avg[0]-S2_LUXFLEX
 * adc_data_avg[1]-S1_LUXFLEX
 * adc_data_avg[2]-S_POT
 * adc_data_avg[3]-BATERIA
 */
//uint32_t test_bateria()
//{
 //divisor de tens�o (8,4 x 11000)/50000 = 1,848 v equivale a 2216,45 ad
 //Carga m�xima = 8,4 v -> tens�o no micro = 1,848 v = 2296,76 adc
 //Carga minima = 7v -> tens�o no micro = 1,54 v   = 1911,466666 adc
 // adc_data_avg[4] x 3,3 v/ 4096

  //uint32_t bateria = ((adc_data_avg[4] * 3.3)/4096);
  //return bateria;
//}

