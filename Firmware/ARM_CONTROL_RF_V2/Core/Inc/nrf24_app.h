#ifndef __NRF24_APP_H
#define __NRF24_APP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum nrf24_role_e
{
	NRF24_APP_ROLE_PTX  = 0,
	NRF24_APP_ROLE_PRX  = 1,
	NRF24_APP_NUM_ROLES = 2,
} nrf24_role_t;

typedef enum nrf24_tx_result_e
{
	NRF24_APP_TIMEOUT = 0,
	NRF24_APP_OK      = 1,
} nrf24_tx_result_t;

typedef struct nrf24_config_s
{
	uint8_t channel;
	nrf24_data_rate_t data_rate;
	nrf24_retransmission_delay_t rdelay;
	uint8_t rcount;
	nrf24_tx_power_t tx_power;
	uint8_t network_address[5];
	uint8_t address_size;
} nrf24_config_t;

typedef void (*nrf24_app_cbk_func)(nRF24_pipe_number_t pipe, uint8_t *buffer, uint8_t length, nrf24_tx_result_t status);

void nrf24_app_init(nrf24_config_t *config, nrf24_app_cbk_func tx_cbk, nrf24_app_cbk_func rx_cbk);
void nrf24_app_tx_data_cbk(nrf24_tx_result_t status);
void nrf24_app_rx_data_cbk(uint8_t pipe);
bool nrf24_app_tx_data(uint8_t *buffer, uint8_t length);
uint8_t nrf24_app_get_pld_size(void);

#ifdef __cplusplus
}
#endif

#endif // __NRF24_APP_H
