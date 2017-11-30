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
  
  Serial.println(""); 
  Serial.print("IP Vremeneske postaje: "); 
  Serial.println(WiFi.localIP()); 
 
}
void loop() {
 String payload; // vrnje niz iz strežnika  
 int httpCode;  // odgovor iz strežniaka
  
 if(WiFi.status()== WL_CONNECTED){   

 // inicializacija:
 // deklariram HTTP clienta (http),
 // določim url za povezavo,
 // dodam json header
   HTTPClient http;    
   http.begin(server_url); 
   http.addHeader("Content-Type", "application/json"); 
 
 // preberem podatke iz DHT senzorja
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // v primeru napake pri branju senzorja DHT
  // izpišem napako
  // zaprem http senzor
  // poskusim vsakih 10s
  // prekinem nadaljevanje izvajanja
   if (isnan(h) || isnan(t)) {
    Serial.println("Napaka: Napaka pri branju podatkov iz senzorja.");
    http.end();  //Close connection
    delay(10000);  //Send a request every 30 seconds      
    return;
  }
 
  // pošljem temperaturo
  // v primeru da je prišlo do napake, izpišem napako, drugale temperaturo
   httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"1\"],\"idlokacija\":[\"4#" + weather_location + "\"],\"vrednost\":["+ String(t) +"]}}");
   payload = http.getString();                  //Get the response payload  
   if(httpCode != 200){    
    Serial.print("Napaka: ");
    Serial.println(payload);    //Print request response payload
   }else{
    Serial.println("Temperatura: "+String(t)+ " C");
   }
   
 
  // pošljem vlago
  // v primeru da je prišlo do napake, izpišem napako,
  // v nasprotnem primeru izpišem vlago.
     httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"2\"],\"idlokacija\":[\"4#" + weather_location + "\"],\"vrednost\":["+ String(h) +"]}}");
     payload = http.getString();                  //Get the response payload  
    if(httpCode != 200){
      payload = http.getString();         
      Serial.print("Napaka: ");
      Serial.println(payload);   
    }else{
      Serial.println("Vlaga: "+String(h)+ "%");
    }

   // Zaprem HTTP clienta
   http.end(); 

 // Izpišem morebiten problem z WIFI povezavo
 }else{
 
    Serial.println("Napaka: Neuspešno povezava v interne.");   
 
 }

 // Izvajam meritev na vsakin time_interval_min minut
  delay(60000 * time_interval_min);  }
