/*
 * client.connect - ESP8266NielitLab1
led control by          client.subscribe  - ESP8266/LED1  and ESP8266/LED2
if we want to publish       client.publish  - ESP8266/LED1_status and  ESP8266/LED2_status   
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

const char* ssid = "demo";
const char* password = "12345678";

//const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_server = "iot.eclipse.org";
//const char* mqtt_server = "broker.mqtt-dashboard.com";
int gpio2_pin = 2;
int gpio0_pin = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    pinMode(gpio2_pin, OUTPUT);
    pinMode(gpio0_pin, OUTPUT);
    digitalWrite(gpio2_pin, HIGH);digitalWrite(gpio0_pin, HIGH);
    delay(500);
    digitalWrite(gpio2_pin, LOW);digitalWrite(gpio0_pin, LOW);

    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    reconnect();
}

void setup_wifi(){

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  if((char)payload[0] == 'o' && (char)payload[1] == 'n' && (char)payload[2] == '1') { //on
     digitalWrite(gpio0_pin, HIGH);
     Serial.print("pin0 high");
  }   
  else if((char)payload[0] == 'o' && (char)payload[1] == 'f' && (char)payload[2] == 'f'&& (char)payload[3] == '1') //off
  {  digitalWrite(gpio0_pin, LOW);
     Serial.print("pin0  low");
  }
  if((char)payload[0] == 'o' && (char)payload[1] == 'n' && (char)payload[2] == '2') { //on
     digitalWrite(gpio2_pin, HIGH);
     Serial.print("pin2 high");
  }   
  else if((char)payload[0] == 'o' && (char)payload[1] == 'f' && (char)payload[2] == 'f'&& (char)payload[3] == '2') //off
  {  digitalWrite(gpio2_pin, LOW);
     Serial.print("pin2  low");
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266NielitLab1")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ESP8266/LED1_status", "Connected!");
      // ... and resubscribe
      client.subscribe("ESP8266/LED1");
      
      client.publish("ESP8266/LED2_status", "Connected!");
      // ... and resubscribe
      client.subscribe("ESP8266/LED2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
