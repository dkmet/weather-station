#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include "settings.h"

#define DHTPIN D4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);

  // Počakam da se postavi ESP
  while(!Serial) { }  
  Serial.println("Zagon");
  

  // pošljem podatke
  sendData();
  
  Serial.println("Spanje " +  String(time_interval_min)+ " min");
  Serial.println("");
  ESP.deepSleep(60e6 * time_interval_min); // spanje time_interval_min minut  
}

void sendData(){
  String payload; // vrnje niz iz strežnika  
  int httpCode;  // odgovor iz strežniaka
  float h;     // vlaga
  float t;     //temperatura
  int sleepIteraction;        //ponovitve spanja
  // Zaženem DHT senzor
   dht.begin(); 
 
 // Povežem na WIFI
  Serial.print("Povezujem se na wifi."); 
  WiFi.begin(wifi_ssid, wifi_password);  
  unsigned long wifiConnectStart = millis(); 
  while (WiFi.status() != WL_CONNECTED) {
   if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("Napaka: Neuspešna povezava. Prosimo preveriti podatke za brezžično povezavo.");
      delay(10000);
   }

    delay(500);
    Serial.print(".");
    if (millis() - wifiConnectStart > 15000) {
      Serial.println("Neuspešna povezava do internega");
      return;
    }
  }
  Serial.println("");  
  Serial.println("WiFi povezava je vzpostavljena");
  Serial.print("Naslov IP: ");
  Serial.println(WiFi.localIP());

      // preberem podatke iz DHT senzorja
  
  h = dht.readHumidity();
  t = dht.readTemperature();
  sleepIteraction =0;
  if(isnan(t)){
    Serial.print("Branje podatkov iz senzorja .");  
    while(isnan(t) && sleepIteraction < 30){
      delay(500);
      Serial.print(".");
      h = dht.readHumidity();
      t = dht.readTemperature();
      sleepIteraction = sleepIteraction +1;
    }  
    Serial.println("");  
  }
 
  
  // v primeru napake pri branju senzorja DHT
  // izpišem napako
  // zaprem http senzor
  // poskusim vsakih 10s
  // prekinem nadaljevanje izvajanja
   if (isnan(h) || isnan(t)) {
    Serial.println("Napaka: Napaka pri branju podatkov iz senzorja.");
    return;
  }

  // Povezava do strežnika
  HTTPClient http;
  http.begin(server_url); 
  http.addHeader("Content-Type", "application/json"); 
  
  // pošljem temperaturo
  // v primeru da je prišlo do napake, izpišem napako, drugale temperaturo
   httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"1\"],\"idlokacija\":[\"4#" + weather_location + "\"],\"vrednost\":["+ String(t) +"]}}");
   payload = http.getString();                  //Get the response payload  
   if(httpCode != 200){    
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
      Serial.println(payload);   
    }else{
      Serial.println("Vlaga: "+String(h)+ "%");
    }

   // Zaprem HTTP clienta
   http.end(); 
}


void loop() {
}

