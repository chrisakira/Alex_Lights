#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

extern "C" {
#include "ComponentCommonDefinitions.h"
#include "DataTableHandler.h"
#include "DataTableSyncHandler.h"
#include "DataTags.h"
}

// Broadcast address (MAC FF:FF:FF:FF:FF:FF)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static volatile bool rx_available = false;
static broadcast_msg_st rx_msg;

// Forward declarations
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len);

// Callback: On data sent status
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0) {
    Serial.println("Delivery success");
    } else {
    Serial.println("Delivery fail");
    }
}

uint8_t sync_send(const broadcast_msg_st* msg)
{
    uint8_t broadcast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t result = esp_now_send(broadcast_addr, (uint8_t*)msg, sizeof(broadcast_msg_st));
    return (result == 0) ? (uint8_t)OK : (uint8_t)NOK;
}

uint8_t sync_recv(broadcast_msg_st* msg)
{
    if (rx_available)
    {
        memcpy(msg, &rx_msg, sizeof(broadcast_msg_st));
        rx_available = false;
        return (uint8_t)OK;
    }
    return (uint8_t)NOK;
}

// Callback: when data is received (register this to receive messages)
void OnDataRecv(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
    char macStr[18];
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    Serial.print("Received packet from: "); Serial.println(macStr);
    Serial.print("Length: "); Serial.println(len);
    return;
}


uint8_t get_textile_id(String mac_address) {
    if (mac_address == "E8:68:E7:CD:42:77") {
        return TEXTILE_1;
    } else if (mac_address == "2C:F4:32:17:45:F8") {
        return TEXTILE_2;
    } else if (mac_address == "4C:11:AE:13:87:40") {
        return TEXTILE_3;
    } else if (mac_address == "10:52:1C:00:37:5E") {
        return TEXTILE_4;
    }

    // Unrecognized MAC
    return (uint8_t)TEXTILE_1;
}
