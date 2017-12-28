## Vremenska postaja 

Oprema:

-ESP8266 NodeMcu 12 (0.9v)

-dht22 (senzor vlage in temperature)

-upornik 4.7K - 10KΩ (uporabljen je 10K)

## Povezava DHT22 z NodeMcu ESP8266

![alt text](https://github.com/dkmet/weather-station/blob/master/images/nodeMcu_dht22.png)
V primeru da senzor dht22 ga poveži na 5v

## Nastavitve

Nastavitve se nahajajo v datoteki "settings.h", ob datoteki weather-station.ino.

```
#define server_url "http://username:geslo@www.moj-url.si/(webRequest)?openAgent&requestType=createNewDoc"
#define wifi_ssid "ssid_name"
#define wifi_password "wifi_password"
#define time_interval_min 1
const String weather_location= "ljubljana";
```
## Upload kode
Pred uploadom kode je potrebno izkllučiti deep sleep RST -D0, na ESPju

## Navodila za delo z github cli

git add README.md

git commit -m "-"

git push

## Zunanje povezave

[Deep-Sleep](https://www.losant.com/blog/making-the-esp8266-low-powered-with-deep-sleep)

[NodeMCU - DHT 11](http://www.iotlearning.net/code/esp8266-code/nodemcu-web-server-showing-dht11-data.php#codesyntax_1)