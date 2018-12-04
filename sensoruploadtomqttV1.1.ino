#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define wifi_ssid "myhome"
#define wifi_password "xerophyte@31527a"
#define mqtt_server "192.168.137.247"  //you MQTT IP Address  //broker.mqtt-dashboard.com
//#define TOKEN "ESP8266_DEMO_TOKEN"

// DHT
#define DHTPIN 2
#define DHTTYPE DHT22

#define temperature_topic "temp/intensity/value"  //Topic temperature  temp/intensity/value  "sensor/temperature" 
#define humidity_topic "hum/intensity/value"        //Topic humidity   hum/intensity/value     "sensor/humidity"



WiFiClient wifiClient;

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;





void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting temperature data.");

  // Reading temperature or humidity takes about 250 milliseconds!
 // float h = dht.readHumidity();
  float h = random(0,100);
  // Read temperature as Celsius (the default)
  //float t = dht.readTemperature();
 float t =  random(0,100);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

 // String temperature = String(t);
 // String humidity = String(h);


 

  // Prepare a JSON payload string
  //String payload = "{";
 // payload += "\"temperature\":"; payload += temperature; payload += ",";
 // payload += "\"humidity\":"; payload += humidity;
 // payload += "}";

  // Send payload
 // char attributes[100];
 // payload.toCharArray( attributes, 100 );
  client.publish(temperature_topic, String(t).c_str(), true);   // Publish temperature on temperature_topic
    client.publish(humidity_topic, String(h).c_str(), true); 
  //Serial.println( attributes );

}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(wifi_ssid, wifi_password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device") ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(10);
  InitWiFi();
  client.setServer( mqtt_server, 1883 );
  lastSend = 0;
}
void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }

  client.loop();
}
