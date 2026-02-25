#include "extras.h"

#include <espnow.h>
#include <Adafruit_VL53L0X.h>

// Global defined in header as extern
uint8_t textile_id = 0;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Send a single [pinIndex, value] command to the expander.
void setPin(uint8_t pinIndex, uint8_t value) {
    delay(1);
    Wire.beginTransmission(I2C_SLAVE_ADDR); 
    Wire.write(pinIndex);
    Wire.write(value);
    Wire.endTransmission();
    delay(1);
}

// Reset all D2–D13 channels to 0.
void clearPins() {
    for (uint8_t i = IDX_D2; i <= IDX_D13; i++) {
        setPin(i, 0);
    }
    setPin(IDX_D44, 0);
    setPin(IDX_D45, 0);
    setPin(IDX_D46, 0);
}

uint32_t get_laser_range(void)
{
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
        if((measure.RangeMilliMeter > MIN_VL_RANGE) && (measure.RangeMilliMeter < MAX_VL_RANGE))
            return 1;
    } else {
        Serial.println(" out of range ");
        return 0;
    }
    return 0;
}

void write_laser_range_to_datatable(void)
{
    uint32_t laser_tag = 0;

    switch (textile_id) {
        case TEXTILE_1:
            laser_tag = TAG_TEXTILE_1_PRESENCE_DETECTED;
            break;
        case TEXTILE_2:
            laser_tag = TAG_TEXTILE_2_PRESENCE_DETECTED;
            break;
        case TEXTILE_3:
            laser_tag = TAG_TEXTILE_3_PRESENCE_DETECTED;
            break;
        case TEXTILE_4:
            laser_tag = TAG_TEXTILE_4_PRESENCE_DETECTED;
            break;
        default:
            laser_tag = TAG_TEXTILE_1_PRESENCE_DETECTED;
    }

    uint32_t range = get_laser_range();
    write_data_table(laser_tag, &range, sizeof(range));
}

// Broadcast address (MAC FF:FF:FF:FF:FF:FF)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static volatile bool rx_available = false;
static broadcast_msg_st rx_msg;

// Callback: On data sent status
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    //Serial.print("Last Packet Send Status: ");
    //if (sendStatus == 0) {
    //    Serial.println("Delivery success");
    //} else {
    //    Serial.println("Delivery fail");
    //}
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

void wifi_boot(void){
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    textile_id = get_textile_id(WiFi.macAddress());
    Serial.print("Device MAC Address: "); Serial.println(WiFi.macAddress());
    // Init ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Set role and register callbacks
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    // Register peer for broadcast (channel 1).
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

}

void datatable_boot(void){
    // Init DataTable
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
}

void wire_boot(void){
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000);
    if (!lox.begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
        while(1);
    }
}

void startup(void){
    // Init Serial Monitor
    Serial.begin(115200);
    wifi_boot();
    datatable_boot();   
    wire_boot();
    Serial.println("ESP-NOW ready. Use sendMessage(msg) or let loop() send periodically.");
}
