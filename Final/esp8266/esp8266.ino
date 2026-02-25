#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "extras.h"
#include "sequence_controller.h"

extern "C" {
#include "ComponentCommonDefinitions.h"
#include "DataTableHandler.h" 
#include "DataTableSyncHandler.h"
#include "DataTags.h"
}


void setup() {
    startup();
}

void loop() {
    static unsigned long lastSeqMs = 0;
    static unsigned long lastSyncMs = 0;
    const unsigned long SEQ_INTERVAL = 100UL; // 100 ms
    const unsigned long SYNC_INTERVAL = 10UL; // 10 ms

    unsigned long now = millis();

    if (now - lastSyncMs >= SYNC_INTERVAL) {
        lastSyncMs = now;
        data_table_sync_handler_loop();
    }

    if (now - lastSeqMs >= SEQ_INTERVAL) {
        lastSeqMs = now;
        loop_sequence(0);
        write_laser_range_to_datatable();
    }

    yield();
}