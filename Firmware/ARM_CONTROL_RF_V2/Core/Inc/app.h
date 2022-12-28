#ifndef APP_H_
#define APP_H_

void app_init(void);
void app_reset_radio(uint8_t rf_channel);
void app_loop(void);
void app_key_pressed(void);

#endif /* APP_H_ */
