#ifndef __EXTRAS_H__
#define __EXTRAS_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "Adafruit_VL53L0X.h"

extern "C" {
#include "ComponentCommonDefinitions.h"
#include "DataTableHandler.h"
#include "DataTableSyncHandler.h"
#include "DataTags.h"
}
 
/******************* GLOBAL *******************/
extern uint8_t textile_id;

/******************* END GLOBAL *******************/

/******************* I2C CODE *******************/
#define I2C_SLAVE_ADDR   0x08
#define MIN_VL_RANGE   100
#define MAX_VL_RANGE   1500

#define SDA_PIN          4
#define SCL_PIN          5

#define IDX_D2           0
#define IDX_D3           1
#define IDX_D4           2
#define IDX_D5           3
#define IDX_D6           4
#define IDX_D7           5
#define IDX_D8           6
#define IDX_D9           7
#define IDX_D10          8
#define IDX_D11          9
#define IDX_D12          10
#define IDX_D13          11
#define IDX_D44          12
#define IDX_D45          13
#define IDX_D46          14

// Exposed helper functions
void setPin(uint8_t pinIndex, uint8_t value);
void clearPins(void);
uint32_t get_laser_range(void);
void write_laser_range_to_datatable(void);
/******************* END I2C CODE *******************/

/******************* ESP NOW CODE *******************/
uint8_t sync_send(const broadcast_msg_st* msg);
uint8_t sync_recv(broadcast_msg_st* msg);
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len);

/******************* END ESP NOW CODE *******************/

uint8_t get_textile_id(String mac_address);

void wifi_boot(void);
void datatable_boot(void);
void wire_boot(void);
void startup(void);

#endif // __EXTRAS_H__