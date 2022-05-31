
#include <WiFi.h>
#include <PubSubClient.h>

#define RELAY01 22
#define RELAY02 23
#define RELAY03 5
#define RELAY04 18
// Update with your Wi-Fi
const char* ssid = "WiFi";
const char* password = "Password";


// Enter your localy running MQTT brokers IP address
//IPAddress mqtt_server(000, 000, 0, 00);
const char* mqtt_server = "broker.mqttdashboard.com";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String relay;
String strTopic;
String strPayload;

//long hexstr_to_value(char *str, unsigned int length) { // converts a hexadecimal value (encoded as ASCII string) to a numeric value
//  char* copy = malloc((sizeof(char) * length) + 1);
//  memcpy(copy, str, sizeof(char) * length);
//  copy[length] = '\0';
//  // the variable "copy" is a copy of the parameter "str". "copy" has an additional '\0' element to make sure that "str" is null-terminated.
//  long value = strtol(copy, NULL, 16);  // strtol converts a null-terminated string to a long value
//  free(copy); // clean up
//  return value;
//}

void setup_wifi() {

  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  //payload[length] = '';
  strTopic = String((char*)topic);
  String messageTemp;
  // you can change topic with whatever you want.
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  if (strTopic == "motor") {

    relay = String(messageTemp);

    if (relay == "0")  {
      Serial.println(" Opening Relay 1");
      digitalWrite(RELAY01, LOW);
      digitalWrite(RELAY02, HIGH);
    }

    if (relay == "1")  {
      Serial.println(" Opening Relay 2");
      digitalWrite(RELAY02, LOW);
      digitalWrite(RELAY01, HIGH);
    }

    if (relay == "2") {
      Serial.println(" Closing Relay 1");
      digitalWrite(RELAY01, HIGH);
      digitalWrite(RELAY02, HIGH);
    }

    if (relay == "3") {
      Serial.println(" Closing Relay 2");
      digitalWrite(RELAY02, HIGH);
      digitalWrite(RELAY01, HIGH);
    }
    if (relay == "4")  {
      Serial.println(" Opening Relay 3");
      digitalWrite(RELAY03, LOW);
      digitalWrite(RELAY04, HIGH);
    }

    if (relay == "5")  {
      Serial.println(" Opening Relay 4");
      digitalWrite(RELAY04, LOW);
      digitalWrite(RELAY03, HIGH);
    }

    if (relay == "6") {
      Serial.println(" Closing Relay 3");
      digitalWrite(RELAY03, HIGH);
      digitalWrite(RELAY04, HIGH);
    }

    if (relay == "7") {
      Serial.println(" Closing Relay 4");
      digitalWrite(RELAY04, HIGH);
      digitalWrite(RELAY03, HIGH);
    }

  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      // you can change topic with whatever you want.
      client.subscribe("motor");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
  client.publish("motor", "NodeMCU Connected!");
}

void setup() {
  pinMode(RELAY01, OUTPUT);
  pinMode(RELAY02, OUTPUT);
  pinMode(RELAY03, OUTPUT);
  pinMode(RELAY04, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
