//Modul: Byt (číslo 1)
//Autor: Martin Chlebovec
//Hardver: Arduino Mega 2560 + Ethernet Wiznet W5100
//Revizia: 4. Mar. 2021
//Zapojenie:

//Sumár úprav revízie:
//Prehľadnejší program, presunutie definicie funkcie pod loop
//Deklaracia pred setupom
//Casovanie requestu cez millis() - presnejsie ako delay, nezastavuje program
//Osetrene pretecenie millis() pouzitim unsigned long premennej timer
//Navysenie casu po vyzadani dat z Onewire zbernic. Pôvodne 1 sekunda, teraz 5 sekund
//Staticke stringy do F makra - ulozenie do flash, nezaberaju RAM

#include <avr\wdt.h>
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#include <DHT_U.h>
#include "SparkFun_Si7021_Breakout_Library.h"
//Definicia typu DHT senzora a digitalnych pinov
#define DHTPIN 2
#define DHTTYPE    DHT22
#define DHTPIN2 3
#define BME280_ADRESA (0x76)

//Definicia zbernic OneWire
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorsA(&oneWire);
#define ONE_WIRE_BUS2 6
OneWire oneWire2(ONE_WIRE_BUS2);
DallasTemperature sensorsB(&oneWire2);
#define ONE_WIRE_BUS3 7
OneWire oneWire3(ONE_WIRE_BUS3);
DallasTemperature sensorsC(&oneWire3);
#define ONE_WIRE_BUS4 8
OneWire oneWire4(ONE_WIRE_BUS4);
DallasTemperature sensorsD(&oneWire4);

DHT_Unified dht(DHTPIN, DHTTYPE);
DHT_Unified dht2(DHTPIN2, DHTTYPE);
Weather sensorsht;
Adafruit_BME280 bme;

byte mac[] = { 0x20, 0xBA, 0x06, 0x75, 0x8C, 0xAA };
const char * host = "www.host.sk";
IPAddress dnServer(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip(192, 168, 0, 45);
EthernetClient client;

unsigned long timer = 0;
void odosli_data();

void setup() {
  //INICIALIZACIA ONEWIRE, SHT21, BME280 A DHT22 SENZOROV
  sensorsA.begin();
  sensorsB.begin();
  sensorsC.begin();
  sensorsD.begin();
  sensor_t sensor;
  sensorsht.begin();
  bme.begin(BME280_ADRESA);
  dht.begin();
  dht2.begin();
  Serial.begin(115200);
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Chyba konfiguracie, manualne nastavenie"));
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  }
  Serial.print(F("Nastavena IP adresa: "));
  Serial.println(Ethernet.localIP());
  wdt_enable(WDTO_8S);
}

void loop() {
  //ETHERNET DROP FIX
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Chyba konfiguracie, manualne nastavenie"));
    wdt_reset();
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  }
  wdt_reset();
  if ((millis() - timer) >= 30000 || timer == 0) { //casovanie kazdych 30 sekund (30000 ms)
    timer = millis();
    //Ethernet.maintain(); //z dokumentacie Ethernet sa moze pouzit, ak sa pouziva DHCP - nie static IP, robi renew IP adresy (moze robit bordel)
    odosli_data();
  }
}

void odosli_data() {
  //MERANIA, DATA OD SENZOROV
  sensorsA.requestTemperatures();
  sensorsB.requestTemperatures();
  sensorsC.requestTemperatures();
  sensorsD.requestTemperatures();
  sensor_t sensor;
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  wdt_reset();
  for (int i = 0; i <= 5; i++) {
    delay(1000);
    wdt_reset();
  }
  wdt_reset();
  String teplota1 = String(sensorsA.getTempCByIndex(0));
  String teplota2 = String(sensorsA.getTempCByIndex(1));
  String teplota3 = String(sensorsB.getTempCByIndex(0));
  String teplota4 = String(sensorsB.getTempCByIndex(1));
  String teplota5 = String(sensorsB.getTempCByIndex(2));
  String teplota6 = String(sensorsB.getTempCByIndex(3));
  String teplota7 = String(sensorsC.getTempCByIndex(0));
  String teplota8 = String(sensorsD.getTempCByIndex(0));
  String vlhkost_sht21 = String(sensorsht.getRH());
  String teplota_sht21 = String(sensorsht.getTemp());
  String vlhkost_bme280 = String(bme.readHumidity());
  String teplota_bme280 = String(bme.readTemperature());

  String teplota_dht = String(event.temperature);
  dht.humidity().getEvent(&event);
  String vlhkost_dht = String(event.relative_humidity);
  sensors_event_t event2;
  dht2.temperature().getEvent(&event2);
  String teplota_dht2 = String(event2.temperature);
  dht2.humidity().getEvent(&event2);
  String vlhkost_dht2 = String(event2.relative_humidity);
  wdt_reset();

  //FORMOVANIE PAYLOAD-U A ODOSIELANIE PO HTTP POST
  String data = "teplota1=" + teplota1 + "&teplota2=" + teplota2 + "&teplota3=" + teplota3 + "&teplota4=" + teplota4 + "&teplota5=" + teplota5 + "&teplota6=" + teplota6 + "&teplota7=" + teplota7 + "&teplota8=" + teplota8 + "&teplota_dht=" + teplota_dht + "&vlhkost_dht=" + vlhkost_dht + "&teplota_dht2=" + teplota_dht2 + "&vlhkost_dht2=" + vlhkost_dht2 + "&teplota_bme280=" + teplota_bme280 + "&vlhkost_bme280=" + vlhkost_bme280 + "&teplota_sht=" + teplota_sht21 + "&vlhkost_sht=" + vlhkost_sht21;
  String url = "/system/arduino/zapishodnoty.php";
  client.stop();
  if (client.connect(host, 80)) {
    Serial.println(F("Odosielam data na webserver"));
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + String(host));
    client.println(F("User-Agent: ArduinoEthernet"));
    client.println(F("Connection: close"));
    client.println(F("Content-Type: application/x-www-form-urlencoded;"));
    client.print(F("Content-Length: "));
    client.println(data.length());
    client.println();
    client.println(data);
    while (client.connected()) {
      //HTTP HEADER
      String line = client.readStringUntil('\n'); //HTTP header
      // Serial.println(line);
      if (line == "\r") {
        break; //blank line between HTTP header and payload
      }
    }
    String line = client.readStringUntil('\n'); //HTTP response
    Serial.println(line);
    if (line = "OK") {
      Serial.println(F("Server potvrdil prijatie dat")); //server poslal OK text ako odpoved na request
    } else {
      Serial.println(F("Server neodpovedal / neplatna odpoved na prijatie dat")); //ziaden output, nieco ine, co neocakavame
    }
  } else {
    Serial.println(F("Neuspesne odoslanie dat - spojenie sa nepodarilo"));
  }
}
