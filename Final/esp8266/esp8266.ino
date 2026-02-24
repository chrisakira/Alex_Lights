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
    startup();
}

void loop() {
    delay(100);
    loop_sequence();
    write_laser_range_to_datatable();
    data_table_sync_handler_loop();
}