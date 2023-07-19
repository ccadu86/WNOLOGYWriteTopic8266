#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "esp";
const char* WIFI_PASSWORD = "teste32esp";

/////////////////////////////////////////////////////////////
const char* MQTT_SERVER = "broker.app.wnology.io";
const char* MQTT_ACCESS_KEY = "cae34e03-ed98-470a-b18d-095efddcdade";
const char* MQTT_ACCESS_SECRET = "adab8d113f9b29b9614756f4f946e4cc4d0bbf61f90a0b8438e1b58d7187f4d8";
const char* DEVICE_ID = "64947c24ce6b4c71f6c8369a";
const char* MQTT_TOPIC = "wnology/64947c24ce6b4c71f6c8369a/state";
const int MQTT_PORT = 1883;
const int PUBLISH_INTERVAL = 300000;

WiFiClient espClient;
PubSubClient client(espClient);

char attributes[200];

void setup() {
  Serial.begin(115200);
  
  connectWiFi();
  connectMQTT();

}

void loop() {
    call_pub(); 
    delay(60000);
}

void connectMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT server...");
    if (client.connect(DEVICE_ID, MQTT_ACCESS_KEY, MQTT_ACCESS_SECRET)) {
      Serial.println("Connected to WEGNOLOGY platform");
      Serial.println("-------------"); 
    } else {
      Serial.print("Failed to connect to WEGNOLOGY platform, error code: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi network");
  Serial.println("-------------"); 
}


void publishData() {
  
  DynamicJsonDocument doc(200);
  doc["data"]["TEMP"] = random(0, 50); ;

  String payload;
  serializeJson(doc, payload);
  payload.toCharArray(attributes, 200);
  Serial.println(payload);

  if (client.connected()) {
      client.publish(MQTT_TOPIC, attributes);
    }
}

void call_pub() {
  if (!client.connected()) {
    Serial.println("Reconnecting to WEGNOLOGY platform");
    connectMQTT();
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi");
    connectWiFi();
  }

  client.loop();
  publishData();
  Serial.println("-------------");    
}
