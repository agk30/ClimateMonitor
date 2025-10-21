// Home Assistant MQTT Discovery topics
#define HA_CONFIG_TEMP "homeassistant/sensor/climate_monitor/temperature/config"
#define HA_CONFIG_HUM "homeassistant/sensor/climate_monitor/humidity/config"
#define HA_STATE_TEMP "homeassistant/sensor/climate_monitor/temperature/state"
#define HA_STATE_HUM "homeassistant/sensor/climate_monitor/humidity/state"

 // Home Assistant IP address
#define MQTT_BROKER "192.168.5.147"
#define MQTT_PORT 1883

// Device unique ID for Home Assistant
#define DEVICE_ID "climate_monitor_001"

// Include necessary headers for function declarations
#include <MQTT.h>

// Function declarations
void sendHomeAssistantDiscovery(MQTTClient &mqttDude);


