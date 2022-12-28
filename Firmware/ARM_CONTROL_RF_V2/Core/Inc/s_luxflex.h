#ifndef INC_S_LUXFLEX_H_
#define INC_S_LUXFLEX_H_

#define ADC_NUM_CHANNELS 4

void adc_calibration(void);
void hw_adc_start(void);
void hw_adc_stop(void);
bool adc_read_channels(uint16_t *adc_val_ch4);

#endif /* INC_S_LUXFLEX_H_ */
