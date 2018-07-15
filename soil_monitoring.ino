#include <Wire.h> 
#include "dht.h"
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

int pHSense = A0;
int sensor = D0;
int output;
const char* ssid = "Mehar";
const char* password = "9999879767";
unsigned long myChannelNumber = 534468;
const char* myWriteAPIKey = "BND35X7GB850XSZD";

WiFiClient WiFiclient;
dht DHT;

void setup() {

  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid,password);
    
    while(WiFi.status()!= WL_CONNECTED){
       delay(500);
       Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi Connected");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
    ThingSpeak.begin(WiFiclient);
    delay(2000);
    pinMode (D6, OUTPUT);
}

void loop(){
    
    output = digitalRead(sensor);
    if (output==1)
    {  digitalWrite(D6, 1);
       Serial.println("Moisture is less than threshold ");
    } 
    else 
     {  digitalWrite(D6, 0);
        Serial.println("Moisture is greater than threshold ");
     }  
    String moisture = (String)output;
    int measuringVal = analogRead(pHSense);
    double vltValue = 5/1024.0 * measuringVal;
    Serial.print("Voltage Value > ");
    Serial.println(vltValue, 3);

    float P0 = 2.1875*vltValue;
    Serial.print("");
    Serial.print("pH Value > ");
    Serial.println(P0, 3);
    Serial.print("");

    DHT.read11(D2);
    Serial.print("Temperature: ");
    Serial.print(DHT.temperature);
    Serial.println("*C");
    Serial.print("Humidity: ");
    Serial.print(DHT.humidity);
    Serial.println("%");
    String temp = (String)DHT.temperature;
    String humidity = (String)DHT.humidity;
    String phValue = (String)P0;
    ThingSpeak.setField(1,temp);
    ThingSpeak.setField(2,humidity);
    ThingSpeak.setField(3,moisture);
    ThingSpeak.setField(4,phValue);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    delay(17000);
    
}
