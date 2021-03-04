//Modul: Kotolna (číslo 2)
//Autor: Martin Chlebovec
//Hardver: Arduino Mega 2560 + Ethernet Wiznet W5100
//Revizia: 4. Mar. 2021

//Sumár úprav revízie:
//Prehľadnejší program, presunutie definicie funkcie pod loop
//Deklaracia pred setupom
//Casovanie requestu cez millis() - presnejsie ako delay, nezastavuje program
//Osetrene pretecenie millis() pouzitim unsigned long premennej timer
//Navysenie casu po vyzadani dat z Onewire zbernic. Pôvodne 1 sekunda, teraz 5 sekund
//Staticke stringy do F makra - ulozenie do flash, nezaberaju RAM
//Rozlisene popisy na UART pre dane funkcie - kedy sa čo preberá / aplikuje

#include <avr\wdt.h>
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//DEFINICIE PORTU A JEDNOTLIVYCH ONEWIRE ZBERNIC
//POCET ZBERNIC: 7
//DEFINOVANE NA D PORTOCH: 22, 23, 24, 25, 26, 27, 28
#define ONE_WIRE_BUS 22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorsA(&oneWire);
#define ONE_WIRE_BUS2 23
OneWire oneWire2(ONE_WIRE_BUS2);
DallasTemperature sensorsB(&oneWire2);
#define ONE_WIRE_BUS3 24
OneWire oneWire3(ONE_WIRE_BUS3);
DallasTemperature sensorsC(&oneWire3);
#define ONE_WIRE_BUS4 25
OneWire oneWire4(ONE_WIRE_BUS4);
DallasTemperature sensorsD(&oneWire4);
#define ONE_WIRE_BUS5 26
OneWire oneWire5(ONE_WIRE_BUS5);
DallasTemperature sensorsE(&oneWire5);
#define ONE_WIRE_BUS6 27
OneWire oneWire6(ONE_WIRE_BUS6);
DallasTemperature sensorsF(&oneWire6);
#define ONE_WIRE_BUS7 28
OneWire oneWire7(ONE_WIRE_BUS7);
DallasTemperature sensorsG(&oneWire7);

//SIEŤOVÁ KONFIGURÁCIA
byte mac[] = { 0xAA, 0xBB, 0xAA, 0xBB, 0xAA, 0xBB };
const char * host = "www.host.sk";
IPAddress dnServer(192, 168, 0, 1); //DNS server
IPAddress gateway(192, 168, 0, 1); //Brana (router)
IPAddress subnet(255, 255, 255, 0); //SUBNET, MASKA
IPAddress ip(192, 168, 0, 45); //IP ADRESA Ethernet shieldu
EthernetClient client; //režim webclient
unsigned long timer = 0;

//DEFINICIE VYSTUPOV
const int rele1 = 29;
const int rele2 = 30;
const int rele3 = 31;
const int rele4 = 32;
const int rele5 = 33;
const int rele6 = 34;
const int rele7 = 35;
const int rele8 = 36;

//DEFINICIA VSTUPOV
const int vstup1 = 37;
const int vstup2 = 38;
const int vstup3 = 39;
const int vstup4 = 40;
const int vstup5 = 41;
const int vstup6 = 42;
const int vstup7 = 43;
const int vstup8 = 44;

//Deklarácia funkcií
void odosli_data();
void odosli_data_vstupy();
void nastavenie_rele1();
void nastavenie_rele2();
void setup() {
  //NASTAVENIE PINU AKO VYSTUPU
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  pinMode(rele5, OUTPUT);
  pinMode(rele6, OUTPUT);
  pinMode(rele7, OUTPUT);
  pinMode(rele8, OUTPUT);

  //POCIATOCNE NASTAVENIE VYSTUPOV --> INVERTOVANA LOGIKA RELE
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
  digitalWrite(rele3, HIGH);
  digitalWrite(rele4, HIGH);
  digitalWrite(rele5, HIGH);
  digitalWrite(rele6, HIGH);
  digitalWrite(rele7, HIGH);
  digitalWrite(rele8, HIGH);

  //NASTAVENIE VYVODU AKO VSTUPU
  pinMode(vstup1, INPUT);
  pinMode(vstup2, INPUT);
  pinMode(vstup3, INPUT);
  pinMode(vstup4, INPUT);
  pinMode(vstup5, INPUT);
  pinMode(vstup6, INPUT);
  pinMode(vstup7, INPUT);
  pinMode(vstup8, INPUT);

  //INICIALIZACIE
  sensorsA.begin(); //inicializacia OneWire 1
  sensorsB.begin(); //inicializacia OneWire 2
  sensorsC.begin(); //inicializacia OneWire 3
  sensorsD.begin(); //inicializacia OneWire 4
  sensorsE.begin(); //inicializacia OneWire 5
  sensorsF.begin(); //inicializacia OneWire 6
  sensorsG.begin(); //inicializacia OneWire 7
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
    wdt_reset();
    nastavenie_rele1();
    wdt_reset();
    nastavenie_rele2();
    wdt_reset();
    odosli_data_vstupy();
  }
}

void odosli_data_vstupy() {
  String hodnota1 = String (digitalRead(vstup1));
  String hodnota2 = String (digitalRead(vstup2));
  String hodnota3 = String (digitalRead(vstup3));
  String hodnota4 = String (digitalRead(vstup4));
  String hodnota5 = String (digitalRead(vstup5));
  String hodnota6 = String (digitalRead(vstup6));
  String hodnota7 = String (digitalRead(vstup7));
  String hodnota8 = String (digitalRead(vstup8));
  wdt_reset();

  //FORMOVANIE PAYLOAD-U A ODOSIELANIE PO HTTP POST
  String data = "vstup1=" + hodnota1 + "&vstup2=" + hodnota2 + "&vstup3=" + hodnota3 + "&vstup4=" + hodnota4 + "&vstup5=" + hodnota5 + "&vstup6=" + hodnota6 + "&vstup7=" + hodnota7 + "&vstup8=" + hodnota8;
  String url = "/system/arduino/zapisvstupov.php";
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
      Serial.println(line);
      if (line == "\r") {
        break; //blank line between HTTP header and payload
      }
    }
    String line = client.readStringUntil('\n'); //HTTP payload
    Serial.println(line);
    if (line = "OK") {
      Serial.println(F("Server potvrdil prijatie dat - vstupy")); //server poslal OK text ako odpoved na request
    } else {
      Serial.println(F("Server neodpovedal / neplatna odpoved na prijatie dat - vstupy")); //ziaden output, nieco ine, co neocakavame
    }
  } else {
    Serial.println(F("Neuspesne odoslanie dat - spojenie sa nepodarilo"));
  }
}

void odosli_data() {
  //VYŽIADANIE DAT, MERANIA, DATA OD SENZOROV
  sensorsA.requestTemperatures();
  sensorsB.requestTemperatures();
  sensorsC.requestTemperatures();
  sensorsD.requestTemperatures();
  sensorsE.requestTemperatures();
  sensorsF.requestTemperatures();
  sensorsG.requestTemperatures();
  wdt_reset();
  for (int i = 0; i <= 5; i++) {
    delay(1000);
    wdt_reset();
  }
  String teplota1 = String(sensorsA.getTempCByIndex(0));
  String teplota2 = String(sensorsA.getTempCByIndex(1));
  String teplota3 = String(sensorsB.getTempCByIndex(0));
  String teplota4 = String(sensorsB.getTempCByIndex(1));
  String teplota5 = String(sensorsC.getTempCByIndex(0));
  String teplota6 = String(sensorsC.getTempCByIndex(1));
  String teplota7 = String(sensorsD.getTempCByIndex(0));
  String teplota8 = String(sensorsD.getTempCByIndex(1));
  String teplota9 = String(sensorsE.getTempCByIndex(0));
  String teplota10 = String(sensorsE.getTempCByIndex(1));
  String teplota11 = String(sensorsF.getTempCByIndex(0));
  String teplota12 = String(sensorsF.getTempCByIndex(1));
  String teplota13 = String(sensorsG.getTempCByIndex(0));
  String teplota14 = String(sensorsG.getTempCByIndex(1));
  String teplota15 = String(sensorsG.getTempCByIndex(2));
  String teplota16 = String(sensorsG.getTempCByIndex(3));
  wdt_reset();

  //FORMOVANIE PAYLOAD-U A ODOSIELANIE PO HTTP POST
  String data = "teplota1=" + teplota1 + "&teplota2=" + teplota2 + "&teplota3=" + teplota3 + "&teplota4=" + teplota4 + "&teplota5=" + teplota5 + "&teplota6=" + teplota6 + "&teplota7=" + teplota7 + "&teplota8=" + teplota8 + "&teplota9=" + teplota9 + "&teplota10=" + teplota10 + "&teplota11=" + teplota11 + "&teplota12=" + teplota12 + "&teplota13=" + teplota13 + "&teplota14=" + teplota14 + "&teplota15=" + teplota15 + "&teplota16=" + teplota16;
  String url = "/system/arduino/zapishodnoty_modul2.php";
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
      Serial.println(line);
      if (line == "\r") {
        break; //blank line between HTTP header and payload
      }
    }
    String line = client.readStringUntil('\n'); //HTTP payload
    Serial.println(line);
    if (line = "OK") {
      Serial.println(F("Server potvrdil prijatie dat - teploty")); //server poslal OK text ako odpoved na request
    } else {
      Serial.println(F("Server neodpovedal / neplatna odpoved na prijatie dat - teploty")); //ziaden output, nieco ine, co neocakavame
    }
  } else {
    Serial.println(F("Neuspesne odoslanie dat - spojenie sa nepodarilo"));
  }
}

void nastavenie_rele1() {
  String url = "/system/arduino/vystupy1.php";
  client.stop();
  if (client.connect(host, 80)) {
    client.print(String("GET ") + url + " HTTP/1.0\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
      if (line == "\r") {
        break;
      }
    }
    String line = client.readStringUntil('\n');
    Serial.print(F("Odpoved webservera - prve 4 vystupy: "));
    Serial.println(line);
    if (line == "0000") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "0001") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else if (line == "0010") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "0011") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else if (line == "0100") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "0101") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else if (line == "0110") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "0111") {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else if (line == "1000") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "1001") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else if (line == "1010") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "1011") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else if (line == "1100") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "1101") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else if (line == "1110") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    } else if (line == "1111") {
      digitalWrite(rele1, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, LOW);
    } else {
      digitalWrite(rele1, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele2, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele3, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele4, HIGH);
    }
    Serial.println(F("Prve 4 vystupy boli synchronizovane a nastavene zo servera"));
  } else {
    Serial.println(F("Problem s pripojenim na webserver - Nepodarilo sa ziskat prve 4 vystupy"));
  }
}

void nastavenie_rele2() {
  String url = "/system/arduino/vystupy2.php";
  client.stop();
  if (client.connect(host, 80)) {
    client.print(String("GET ") + url + " HTTP/1.0\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
      if (line == "\r") {
        break;
      }
    }
    String line = client.readStringUntil('\n');
    Serial.print(F("Odpoved webservera - druhe 4 vystupy: "));
    Serial.println(line);
    if (line == "0000") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "0001") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else if (line == "0010") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "0011") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else if (line == "0100") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "0101") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else if (line == "0110") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "0111") {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else if (line == "1000") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "1001") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else if (line == "1010") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "1011") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else if (line == "1100") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "1101") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else if (line == "1110") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    } else if (line == "1111") {
      digitalWrite(rele5, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, LOW);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, LOW);
    } else {
      digitalWrite(rele5, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele6, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele7, HIGH);
      delay(1000);
      wdt_reset();
      digitalWrite(rele8, HIGH);
    }
    Serial.println(F("Druhe 4 vystupy boli synchronizovane a nastavene zo servera"));
  } else {
    Serial.println(F("Problem s pripojenim na webserver - Nepodarilo sa ziskat druhe 4 vystupy"));
  }
}
