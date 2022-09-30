#include <ESP8266WiFi.h>
#include "DHT.h"


#define DHT1PIN D1
#define DHT2PIN D2
#define DHTTYPE DHT22
#define LDR1PIN D5
#define LDR2PIN D7
#define MOISTURE1PIN D6
#define MOISTURE2PIN A0
#define RELAYMODULE4CH_PIN_IN1  D4  //pump
#define RELAYMODULE4CH_PIN_IN2  D8  //humidifier
#define RELAYMODULE4CH_PIN_IN3  D3  //fans
#define RELAYMODULE4CH_PIN_IN4  D0  //bulb

#define faltupin SD0
#define faltupin2 SDC
#define faltupin3 SDK

#define sendingled D8

 
int RelayModule4chPins[] = { RELAYMODULE4CH_PIN_IN1, RELAYMODULE4CH_PIN_IN2, RELAYMODULE4CH_PIN_IN3, RELAYMODULE4CH_PIN_IN4 };
const char* ssid     = "birdcage";
const char* password = "birdcage";
const char* host = "api.birdcage.tk";
DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);

void setup() {
    pinMode(RELAYMODULE4CH_PIN_IN1, OUTPUT); 
    pinMode(RELAYMODULE4CH_PIN_IN2, OUTPUT);
    pinMode(RELAYMODULE4CH_PIN_IN3, OUTPUT);
    pinMode(RELAYMODULE4CH_PIN_IN4, OUTPUT);
    pinMode(sendingled,OUTPUT);

  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
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
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());




  
}
void loop() {
WiFiClient client;

  const int httpPort = 80;
for (int i = 0; i < 4; i++) { 
  Serial.println("checking");

    digitalWrite(RelayModule4chPins[i],LOW);
  delay(500);
    digitalWrite(RelayModule4chPins[i],HIGH);
  
    }
digitalWrite(sendingled,LOW);

  
int  ldr1_value = analogRead(LDR1PIN);
int  ldr2_value = analogRead(LDR2PIN);
  
  Serial.println("Light Sensor1");
  Serial.println(ldr1_value);
  Serial.println("Light Sensor2");
  Serial.println(ldr2_value);

 int sm1_value = digitalRead(MOISTURE1PIN);   //soil moisture
 int sm2_value = analogRead(MOISTURE2PIN);   //soil moisture
  Serial.println("Soil Moisture1");
  Serial.println(sm1_value);
  Serial.println("Soil Moisture2");
  Serial.println(sm2_value);
  
  float h1 = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float t1 = dht1.readTemperature();
  if (isnan(h1) || isnan(t1)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  Serial.println("Humidity1");
    Serial.println(h1);
   Serial.println("Temperature1");
   Serial.println(t1);

    float h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  float t2 = dht2.readTemperature();
  if (isnan(h2) || isnan(t2)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  Serial.println("Humidity2");
    Serial.println(h2);
  Serial.println("Temperature2");
    Serial.println(t2);

   
 String farm_name = "Sir%20Fawad%20Farm";
 String chamber_name = "Patio";
 String zone_name = "Zone_2";
 String url;

 /////////////////////////////

 Serial.print("connecting to ");
  Serial.println(host);

   
    
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
 }
 
 if(ldr1_value >= 500){
   url = "/insertstream.php?component_name=" + String("Light%20Sensor1") + "&component_id="+ String(41) + "&component_value="+ String("Low") + "&zone_name="+ String(zone_name)+"&zone_id="+ String(35)+"&chamber_name="+ String(chamber_name)+"&farm_name="+ String(farm_name);
 }
 else{
//   url = "/insertstream.php?component_name=" + String("Light%20Sensor1") + "&component_value="+ String("High")+ "&zone_name="+ String(zone_name)+"&chamber_name="+ String(chamber_name)+"&farm_name="+ String(farm_name);
     url = "/insertstream.php?component_name=" + String("Light%20Sensor1") + "&component_id="+ String(41) + "&component_value="+ String("High") + "&zone_name="+ String(zone_name)+"&zone_id="+ String(35)+"&chamber_name="+ String(chamber_name)+"&farm_name="+ String(farm_name);

  }
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
   digitalWrite(sendingled,HIGH);
  delay(2500);
  digitalWrite(sendingled,LOW); 
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");




///////////////////////////////////////////////////////

  

    Serial.println("Going to sleep for 30 minutes");
    delay(3000);

////////////////////////////////////////////////////////////
}
