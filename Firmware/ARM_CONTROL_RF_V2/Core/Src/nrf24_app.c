#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "hw.h"
#include "nrf24.h"
#include "nrf24_app.h"

const uint8_t MAX_PAYLOAD_LEN = 32;

static void all_prx_cfg(nrf24_config_t *config);

typedef struct nrf24_driver_s nrf24_driver_t;

typedef struct nrf24_tx_int_s
{
	void (*cbk)(nrf24_driver_t *driver, nrf24_tx_result_t status);
} nrf24_tx_int_t;

typedef struct nrf24_rx_int_s
{
	void (*cbk)(nrf24_driver_t *driver, uint8_t pipe);
} nrf24_rx_int_t;

typedef struct nrf24_cfg_s
{
	void (*cbk)(nrf24_config_t *config);
} nrf24_cfg_t;

typedef struct nrf24_driver_s
{
	nrf24_tx_int_t tx_int;
	nrf24_rx_int_t rx_int;
	nrf24_cfg_t cfg[NRF24_APP_NUM_ROLES];
	void (*pre_common_cfg)(nrf24_config_t *config);
	void (*post_common_cfg)(nrf24_config_t *config);
	nrf24_app_cbk_func client_tx_cbk;
	nrf24_app_cbk_func client_rx_cbk;
	nrf24_role_t role;
	nrf24_config_t config;
} nrf24_driver_t;

nrf24_driver_t *driver = NULL;

#if 0
UART_HandleTypeDef huart3;
#define UART_SendStr(x) HAL_UART_Transmit(&huart3,(uint8_t *)x,strlen((char *)x),100)
#define UART_SendInt(x) { char buf[32]; sprintf(buf,"%d",(int) x); UART_SendStr(buf); }
#define UART_SendBufHex(x,y) { char buf[10]; unsigned int n; for(n = 0 ; n < y ; n++){sprintf(buf,"0x%02X ",(int)x[n]); UART_SendStr(buf);} UART_SendStr("\n"); }
#else
#define UART_SendStr(x)
#define UART_SendInt(x)
#define UART_SendBufHex(x,y)

#endif

static void all_all_tx_int(nrf24_driver_t *driver, nrf24_tx_result_t status)
{
	nrf24_app_cbk_func cbk = driver->client_tx_cbk;

	if(cbk)
		cbk(nRF24_PIPE_INVALID, 0, 0, status);

	driver->role = NRF24_APP_ROLE_PRX;
	driver->cfg[driver->role].cbk(&driver->config);
}

static void all_all_rx_int(nrf24_driver_t *driver, uint8_t pipe)
{
	uint8_t len = 0;
	uint8_t buffer[MAX_PAYLOAD_LEN];
	nrf24_app_cbk_func cbk = driver->client_rx_cbk;
//	nRF24_CE_L();

	// Pass PTX request to application
	nRF24_ReadPipePayload(pipe, buffer, &len);

	if(cbk)
		cbk(pipe, buffer, MAX_PAYLOAD_LEN, NRF24_APP_OK);

//	nRF24_CE_H();
}

void nrf24_app_tx_data_cbk(nrf24_tx_result_t status)
{
	driver->tx_int.cbk(driver, status);
}

void nrf24_app_rx_data_cbk(uint8_t pipe)
{
	driver->rx_int.cbk(driver, pipe);
}

static void all_ptx_cfg(nrf24_config_t *config)
{
//	nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR_PRX);
//	nRF24_SetAddr(nRF24_PIPE1, nRF24_ADDR_PTX);
//	nRF24_DisableAA(nRF24_PIPE1);
//	nRF24_EnableAA(nRF24_PIPETX);
//	nRF24_SetOperationalMode(nRF24_MODE_TX);

	nRF24_SetAddr(nRF24_PIPETX, config->network_address);
	nRF24_SetAddr(nRF24_PIPE0, config->network_address);
	nRF24_EnableAA(nRF24_PIPE0);
	nRF24_SetOperationalMode(nRF24_MODE_TX);

	hw_nrf24_ce_high();

	nRF24_FlushTX();
	nRF24_FlushRX();

	hw_timer_delay_us(140);
}

static void all_prx_cfg(nrf24_config_t *config)
{
//	nRF24_SetAddr(nRF24_PIPETX, nRF24_ADDR_PTX); // program address for pipe
//	nRF24_SetAddr(nRF24_PIPE1, nRF24_ADDR_PRX); // program address for pipe
//	nRF24_DisableAA(nRF24_PIPETX);
//	nRF24_SetRXPipe(nRF24_PIPE1, nRF24_AA_ON, MAX_PAYLOAD_LEN); // Auto-ACK: enabled, payload length: MAX_PAYLOAD_LEN bytes
//	nRF24_SetOperationalMode(nRF24_MODE_RX);

	nRF24_SetAddr(nRF24_PIPE0, config->network_address); // program address for pipe
	nRF24_SetRXPipe(nRF24_PIPE0, nRF24_AA_ON, MAX_PAYLOAD_LEN); // Auto-ACK: enabled, payload length: MAX_PAYLOAD_LEN bytes
	nRF24_SetOperationalMode(nRF24_MODE_RX);

	hw_nrf24_ce_high();

	nRF24_FlushTX();
	nRF24_FlushRX();

	hw_timer_delay_us(140);
}

static void all_all_pre_common_cfg(nrf24_config_t *config)
{
	nRF24_SetRFChannel(config->channel);
	nRF24_SetDataRate(config->data_rate);
	nRF24_SetCRCScheme(nRF24_CRC_2byte);
	nRF24_SetAddrWidth(config->address_size);
	nRF24_SetAutoRetr(config->rdelay,config->rcount);
	nRF24_SetTXPower(config->tx_power);
	nRF24_SetFeatureReg(0);
	//nRF24_SetFeatureReg(nRF24_MASK_EN_DPL | nRF24_MASK_EN_ACK_PAY);
}

static void all_all_post_common_cfg(nrf24_config_t *config)
{
	hw_nrf24_ce_high();
	nRF24_SetPowerMode(nRF24_PWR_UP);
	nRF24_ClearIRQFlags();
	hw_nrf24_interrupt_set_state(true);
}

static nrf24_driver_t driver_inst =
	{
		.tx_int = { .cbk = all_all_tx_int },
		.rx_int = { .cbk = all_all_rx_int },
		.cfg    = { [0].cbk = all_ptx_cfg, [1].cbk = all_prx_cfg },
		.pre_common_cfg = all_all_pre_common_cfg,
		.post_common_cfg = all_all_post_common_cfg,
		.client_tx_cbk = 0,
		.client_rx_cbk = 0,
	};

uint8_t nrf24_app_get_pld_size(void)
{
	return MAX_PAYLOAD_LEN;
}

bool nrf24_app_tx_data(uint8_t *pBuf, uint8_t length)
{
	if(driver->role == NRF24_APP_ROLE_PRX)
	{
		driver->role = NRF24_APP_ROLE_PTX;
		driver->cfg[driver->role].cbk(&driver->config);

		nRF24_WritePayload(pBuf, length);

//		nRF24_CE_H(); // CE must be held high at least 10us
//		HAL_Delay_us(15);
//		nRF24_CE_L();

		return true;
	}
	else // there is some pending transmission !
		return false;
}

void nrf24_app_init(nrf24_config_t *config, nrf24_app_cbk_func tx_cbk, nrf24_app_cbk_func rx_cbk)
{
	hw_nrf24_init();

	driver = &driver_inst;
	memcpy(&driver->config,config,sizeof(nrf24_config_t));

	driver->role = NRF24_APP_ROLE_PRX;
	driver->client_tx_cbk = tx_cbk;
	driver->client_rx_cbk = rx_cbk;

	driver->pre_common_cfg(&driver->config);
	driver->cfg[driver->role].cbk(&driver->config);
	driver->post_common_cfg(&driver->config);
}
