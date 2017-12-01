## Vremenska postaja 

ESP8266 Vremenska postaja

Oprema:

-ESP NodeMcu 12 (0.9v)

-dht22 (senzor vlage in temperature)

-upornik 4.7K - 10KΩ (uporabljen je 10K)

## Navodila za delo z github cli

git add README.md

git commit -m "-"

git push

## Povezava DHT22 z NodeMcu ESP8266

![alt text](https://github.com/dkmet/weather-station/blob/master/images/nodeMcu_dht22.png)

## Nastavitve

Nastavitve se nahajajo v datoteki "settings.h"

```
#define server_url "http://username:geslo@www.moj-url.si/(webRequest)?openAgent&requestType=createNewDoc"
#define wifi_ssid "ssid_name"
#define wifi_password "wifi_password"
#define time_interval_min 1
const String weather_location= "ljubljana";
```