#ifndef HW_H_
#define HW_H_

#define HW_SENSOR_PID_SIZE    16
#define HW_SENSOR_TAG_SIZE    16
#define HW_NRF24_MAX_CHANNELS 9
#define HW_NRF24_CHANNEL_RETRIES 4

#define HW_NRF23_CHANNEL_CALC(x) (2456+13*(( (x) % HW_NRF24_MAX_CHANNELS) - 4) - 2400)
void hw_init(void);
void hw_tick_1ms(void);
void hw_update_trigger(void);

void hw_timer_init_us(void);
void hw_timer_delay_us(uint32_t delay);
uint32_t hw_timer_elapsed_us(uint32_t start);
uint32_t hw_timer_get_tick_us(void);
uint32_t hw_timer_get_tick_ms(void);
uint32_t hw_timer_elapsed_ms(uint32_t start);

void hw_nrf24_init(void);
uint8_t hw_nrf24_spi_rw(uint8_t data);
void hw_nrf24_interrupt_set_state(bool state);
void hw_nrf24_ce_low(void);
void hw_nrf24_ce_high(void);
void hw_nrf24_csn_low(void);
void hw_nrf24_csn_high(void);
void hw_nrf24_set_channel(uint8_t channel);
void hw_nrf24_txen_state(bool state);
void hw_nrf24_rxen_state(bool state);

void hw_power_on(void);
void hw_power_off(void);

uint16_t hw_battery_level(void);
void hw_adc_set_input(bool state);
void hw_uart_tx(uint8_t *buffer, uint32_t size);
void hw_sleep(void);

void hw_user_led_toggle(void);
void hw_user_led_set(bool state);

#endif /* HW_H_ */
