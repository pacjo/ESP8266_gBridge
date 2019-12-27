#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char *WIFI_SSID = "";                         //Your WiFi SSID
const char *WIFI_Password = "";                     //Your WiFi Password
const char *MQTT_Server = "mqtt.gbridge.io";        //MQTT server adress, you can change it
const char *MQTT_Username = "";                     //Your gBridge username
const char *MQTT_Password = "";                     //Your gBridge Password
const char *MQTT_device_ID = "";                    //Your device "name" (not one from dashboard), you can write whatever you want
const char *MQTT_Topic = "";                        //MQTT topic that your device uses  e.g. "gBridge/u0001/d00001/onoff"
const char *MQTT_Topic_set = "";                    //Just add "/set" to MQTT_Topic

//******************************************************************************************************************

void WIFI_Connect() {

  WiFi.begin(WIFI_SSID, WIFI_Password);

  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
  
  delay(500);
  Serial.print(".");
  
  }
  
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

//******************************************************************************************************************

void reconnect() {
  
  while (!client.connected()) {
  
  Serial.print("Attempting MQTT connection...");
  
  if (client.connect(MQTT_device_ID, MQTT_Username, MQTT_Password)) {
  
  Serial.println("connected");
  client.subscribe(MQTT_Topic);
  
  } else {
  
  Serial.print("failed with state: ");
  Serial.print(client.state());
  Serial.println(" trying again in 5 seconds");
  delay(5000);
  
  }
  
  }
}

//******************************************************************************************************************

void callback(char *topic, byte *payload, unsigned int length) {
  
  String message = "";
  
  for (int i = 0; i < length; i++) {
  
  //Serial.print(payload[i]);    //Enable for testing purposes
  message += payload[i];
  
  }

if (message == "??") {          //Change the "??" with message you get from gBridge, one for switching on  (to do that just uncomment  "//Serial.print(payload[i]);" above and check what you are getting)
  
  Serial.println("Switching output ON");
  client.publish(MQTT_Topic_set, "1");
  
}

if (message == "??") {          //Change the "??" with message you get from gBridge, one for switching off  (to do that just uncomment  "//Serial.print(payload[i]);" above and check what you are getting)
  
  Serial.println("Switching output OFF");
  client.publish(MQTT_Topic_set, "0");
  
}

}

//******************************************************************************************************************

void setup() {
  
  Serial.begin(9600);
  delay(2000);

  WIFI_Connect();
  client.setServer(MQTT_Server, 1883);
  client.setCallback(callback);
  
}

void loop() {
  
  if (!client.connected()) {  //Connecting to MQTT broker
  
  reconnect();
  
  }
  
  client.loop();
  
}
