#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "extras.h"
extern "C" {
#include "ComponentCommonDefinitions.h"
#include "DataTableHandler.h"
#include "DataTableSyncHandler.h"
#include "DataTags.h"
}


void setup() {
    // Init Serial Monitor
    Serial.begin(115200);
    delay(10000);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
    }

    // Set role and register callbacks
    // For ESP8266, the role can be controller or slave. We'll set controller
    // so this sketch can both send and receive broadcast messages.
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    // Register peer for broadcast (channel 1). A broadcast peer may not be
    // strictly required, but adding it maintains compatibility.
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);


    // Init DataTable
    // if (init_data_table_handler((uint8_t)get_textile_id(WiFi.macAddress())) != (uint8_t)OK)
    if (init_data_table_handler((uint8_t)1) != (uint8_t)OK)
    {
        Serial.println("DataTable init failed!");
        while(1) delay(1000);
    }

    // Init Sync Handler tag lists (required before build_broadcast_buffer works)
    init_data_table_sync_handler();
    // Init Sync Handler with callbacks
    if (init_data_table_sync_handler_comm(sync_send, sync_recv, 250) != (uint8_t)OK)
    {
        Serial.println("Sync Handler init failed!");
        while(1) delay(1000);
    }
  Serial.println("ESP-NOW ready. Use sendMessage(msg) or let loop() send periodically.");
}

void loop() {
    delay(100);

    static uint32_t data_1, data_2, data_3;
    
    write_data_table(TAG_TEXTILE_1_PREVIOUS_SEQUENCE_STATE, &data_1,  sizeof(data_1)); data_1++;
    write_data_table(TAG_TEXTILE_1_ACTUAL_SEQUENCE_STATE,   &data_2,  sizeof(data_2)); data_2++;
    write_data_table(TAG_TEXTILE_1_PRESENCE_DETECTED,       &data_3,  sizeof(data_3)); data_3++;
    data_table_sync_handler_loop();
}