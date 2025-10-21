#include <MQTT.h>
#include "mqttHelper.h"


void sendHomeAssistantDiscovery(MQTTClient &mqttDude) {
  Serial.println("=== Starting Home Assistant MQTT Discovery ===");
  
  // Temperature Sensor Discovery Configuration
  Serial.println("Configuring Temperature Sensor...");
  
  String tempDiscoveryTopic = String(HA_CONFIG_TEMP);
  String tempConfig = "{";
  tempConfig += "\"name\":\"Climate Monitor Temperature\",";
  tempConfig += "\"device_class\":\"temperature\",";
  tempConfig += "\"state_topic\":\"" + String(HA_STATE_TEMP) + "\",";
  tempConfig += "\"unit_of_measurement\":\"°C\",";
  tempConfig += "\"value_template\":\"{{ value }}\",";
  tempConfig += "\"unique_id\":\"" + String(DEVICE_ID) + "_temperature\",";
  tempConfig += "\"object_id\":\"climate_monitor_temperature\",";
  tempConfig += "\"device\":{";
  tempConfig += "\"identifiers\":[\"" + String(DEVICE_ID) + "\"],";
  tempConfig += "\"name\":\"Climate Monitor\",";
  tempConfig += "\"model\":\"DHT22 Climate Sensor\",";
  tempConfig += "\"manufacturer\":\"DIY Arduino\",";
  tempConfig += "\"sw_version\":\"1.0\",";
  tempConfig += "\"hw_version\":\"Arduino Uno R4 WiFi\"";
  tempConfig += "},";
  tempConfig += "\"availability\":{";
  tempConfig += "\"topic\":\"homeassistant/sensor/climate_monitor/availability\",";
  tempConfig += "\"payload_available\":\"online\",";
  tempConfig += "\"payload_not_available\":\"offline\"";
  tempConfig += "},";
  tempConfig += "\"icon\":\"mdi:thermometer\"";
  tempConfig += "}";
  
  Serial.println("Publishing temperature discovery to: " + tempDiscoveryTopic);
  Serial.println("Config payload length: " + String(tempConfig.length()));
  
  if (mqttDude.publish(tempDiscoveryTopic.c_str(), tempConfig.c_str(), true, 0)) {  // retained = true, qos = 0
    Serial.println("✓ Temperature sensor discovery sent successfully");
  } else {
    Serial.println("✗ Failed to send temperature sensor discovery");
  }
  
  delay(1000);  // Wait between discovery messages
  
  // Humidity Sensor Discovery Configuration
  Serial.println("Configuring Humidity Sensor...");

  String humDiscoveryTopic = String(HA_CONFIG_HUM);
  String humConfig = "{";
  humConfig += "\"name\":\"Climate Monitor Humidity\",";
  humConfig += "\"device_class\":\"humidity\",";
  humConfig += "\"state_topic\":\"" + String(HA_STATE_HUM) + "\",";
  humConfig += "\"unit_of_measurement\":\"%\",";
  humConfig += "\"value_template\":\"{{ value }}\",";
  humConfig += "\"unique_id\":\"" + String(DEVICE_ID) + "_humidity\",";
  humConfig += "\"object_id\":\"climate_monitor_humidity\",";
  humConfig += "\"device\":{";
  humConfig += "\"identifiers\":[\"" + String(DEVICE_ID) + "\"],";
  humConfig += "\"name\":\"Climate Monitor\",";
  humConfig += "\"model\":\"DHT22 Climate Sensor\",";
  humConfig += "\"manufacturer\":\"DIY Arduino\",";
  humConfig += "\"sw_version\":\"1.0\",";
  humConfig += "\"hw_version\":\"Arduino Uno R4 WiFi\"";
  humConfig += "},";
  humConfig += "\"availability\":{";
  humConfig += "\"topic\":\"homeassistant/sensor/climate_monitor/availability\",";
  humConfig += "\"payload_available\":\"online\",";
  humConfig += "\"payload_not_available\":\"offline\"";
  humConfig += "},";
  humConfig += "\"icon\":\"mdi:water-percent\"";
  humConfig += "}";
  
  Serial.println("Publishing humidity discovery to: " + humDiscoveryTopic);
  Serial.println("Config payload length: " + String(humConfig.length()));
  
  if (mqttDude.publish(humDiscoveryTopic.c_str(), humConfig.c_str(), true, 0)) {  // retained = true, qos = 0
    Serial.println("✓ Humidity sensor discovery sent successfully");
  } else {
    Serial.println("✗ Failed to send humidity sensor discovery");
  }
  
  delay(1000);
  
  // Send device availability as online
  String availabilityTopic = "homeassistant/sensor/climate_monitor/availability";
  if (mqttDude.publish(availabilityTopic.c_str(), "online", true, 0)) {
    Serial.println("✓ Device availability status sent");
  } else {
    Serial.println("✗ Failed to send availability status");
  }
  
  Serial.println("=== Home Assistant Discovery Complete ===");
  Serial.println("Check Home Assistant -> Settings -> Devices & Services -> MQTT");
  Serial.println("Your device should appear as 'Climate Monitor'");
  Serial.println();
}