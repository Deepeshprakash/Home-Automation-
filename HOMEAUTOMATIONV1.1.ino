

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
//#include <ArduinoOTA.h>



//EDIT THESE LINES TO MATCH YOUR SETUP
#define MQTT_SERVER "192.168.137.247"  //you MQTT IP Address  //broker.mqtt-dashboard.com
const char* ssid = "myhome";
const char* password = "xerophyte@31527a";


const int switchPin1 = D1;
const int switchPin2 = D2;
const int switchPin3 = D3;
const int switchPin4 = D5;


char const* switchTopic1 = "myhouse/MBL/light";    //"/house/switch1/";
char const* switchTopic2 = "myhouse/MBAC/light";     //"/house/switch2/"; 
char const* switchTopic3 = "myhouse/MBTV/light";     //"/house/switch3/";
char const* switchTopic4 = "myhouse/MBTL/light";     // "/house/switch4/";


WiFiClient wifiClient;

void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);


void callback(char* topic, byte* payload, unsigned int length) {

  //convert topic to string to make it easier to work with
  String topicStr = topic; 
  
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);

   if (topicStr == "myhouse/MBL/light") 
    {

     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     if(payload[0] == '1'){
       digitalWrite(switchPin1, HIGH);
       Serial.println("Switch 1 is on");
      // client.publish("/house/switchConfirm1/", "1");
       }

      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       digitalWrite(switchPin1, LOW);
       Serial.println("Switch 1 is off");
     //  client.publish("/house/switchConfirm1/", "0");
       }
     }

     // EJ: copy and paste this whole else-if block, should you need to control more switches
     else if (topicStr == "myhouse/MBAC/light") 
     {
     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     if(payload[0] == '1'){
       digitalWrite(switchPin2, HIGH);
       Serial.println("Switch 2 is on");
     //  client.publish("/house/switchConfirm2/", "1");
       }

      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       digitalWrite(switchPin2, LOW);
       Serial.println("Switch 2 is off");
     //  client.publish("/house/switchConfirm2/", "0");
       }
     }
     else if (topicStr == "myhouse/MBTV/light") 
     {
     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     if(payload[0] == '1'){
       digitalWrite(switchPin3, HIGH);
       Serial.println("Switch 3 is on");
      // client.publish("/house/switchConfirm3/", "1");
       }

      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       digitalWrite(switchPin3, LOW);
       Serial.println("Switch 3 is off");
      // client.publish("/house/switchConfirm3/", "0");
       }
     }
     else if (topicStr == "myhouse/MBTL/light") 
     {
     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     if(payload[0] == '1'){
       digitalWrite(switchPin4, HIGH);
       Serial.println("Switch 4 is on");
      // client.publish("/house/switchConfirm4/", "1");
       }

      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       digitalWrite(switchPin4, LOW);
       Serial.println("Switch 4 is off");
     //  client.publish("/house/switchConfirm4/", "0");
       }
     }
}

//String macToStr(const uint8_t* mac){

// String result;

//  for (int i = 0; i < 6; ++i) {
 //   result += String(mac[i], 16);

 //   if (i < 5){
  //    result += ':';
  //  }
  //}

 // return result;
//}
void reconnect() {

  //attempt to connect to the wifi if connection is lost
  if(WiFi.status() != WL_CONNECTED){
    //debug printing
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //loop while we wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    //print out some more debug once connected
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  //make sure we are connected to WIFI before attemping to reconnect to MQTT
  if(WiFi.status() == WL_CONNECTED){
  // Loop until we're reconnected to the MQTT server
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      // Generate client name based on MAC address and last 8 bits of microsecond counter
     // String clientName;
     // clientName += "esp8266-";
    //  uint8_t mac[6];
    //  WiFi.macAddress(mac);
     // clientName += macToStr(mac);

      //if connected, subscribe to the topic(s) we want to be notified about
      //EJ: Delete "mqtt_username", and "mqtt_password" here if you are not using any 
      if (client.connect("NODEMCU")) {  //EJ: Update accordingly with your MQTT account 
        Serial.print("\tMQTT Connected");
        client.subscribe(switchTopic1);
        client.subscribe(switchTopic2);
        client.subscribe(switchTopic3);
        client.subscribe(switchTopic4);
        //EJ: Do not forget to replicate the above line if you will have more than the above number of relay switches
      }

      //otherwise print failed for debugging
     else{Serial.println("\tFailed."); abort();}
    }
  }
}

//generate unique name from MAC addr


void setup() {
  //initialize the switch as an output and set to LOW (off)
  pinMode(switchPin1, OUTPUT); // Relay Switch 1
  digitalWrite(switchPin1, LOW);

  pinMode(switchPin2, OUTPUT); // Relay Switch 2
  digitalWrite(switchPin2, LOW);

  pinMode(switchPin3, OUTPUT); // Relay Switch 3
  digitalWrite(switchPin3, LOW);

  pinMode(switchPin4, OUTPUT); // Relay Switch 4
  digitalWrite(switchPin4, LOW);

  //ArduinoOTA.setHostname("My Arduino WEMO"); // A name given to your ESP8266 module when discovering it as a port in ARDUINO IDE
 // ArduinoOTA.begin(); // OTA initialization

  //start the serial line for debugging
  Serial.begin(115200);
  delay(100);

  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();

  //wait a bit before starting the main loop
      delay(2000);
}


void loop(){

  //reconnect if connection is lost
  if (!client.connected() && WiFi.status() == 3) 
  {
      reconnect();
      }

  //maintain MQTT connection
  client.loop();

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(10); 
//  ArduinoOTA.handle();
}
