#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include "settings.h"

#define DHTPIN D4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
 
  Serial.begin(115200);                 //Serial connection
  WiFi.begin(wifi_ssid, wifi_password);   //WiFi connection
  dht.begin(); 

  // povezovanje na WIFI
  Serial.print("Povezujem se na wifi."); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("."); 
  }
  
  Serial.print(""); 
  Serial.println("Povezava je bila uspesno vzpostavljena: "); 
  Serial.println(WiFi.localIP()); 
 
}
void loop() {
   
 if(WiFi.status()== WL_CONNECTED){   

 // inicializacija:
 // deklariram HTTP clienta (http),
 // določim url za povezavo,
 // dodam json header
   HTTPClient http;    
   http.begin(server_url); 
   http.addHeader("Content-Type", "application/json"); 
 
 // preberem podatke iz 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // float f = dht.readTemperature(true); //Farahaid
  Serial.println(t);
   
   if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    http.end();  //Close connection
    delay(10000);  //Send a request every 30 seconds      
    return;
  }
  
  // pošljem temperaturo
   int httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"1\"],\"idlokacija\":[\"4#ljubljana\"],\"vrednost\":["+ String(t) +"]}}");
   String payload = http.getString();                  //Get the response payload 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload

// pošljem vlago
  httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"2\"],\"idlokacija\":[\"4#ljubljana\"],\"vrednost\":["+ String(h) +"]}}");
  payload = http.getString();                  //Get the response payload 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload

 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
 
  delay(60000);  //Send a request every 30 seconds
 
}
