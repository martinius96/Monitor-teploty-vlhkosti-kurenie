# Monitor teplôt, vlhkostí, ovládanie ventilov - dokumentácia
* Systém využíva 2x Arduino Mega 2560 s 2x Ethernet shieldmi Wiznet W5100
* **Modul 1 - BYT** je monitorovací modul, ktorý sníma teploty na jednom z poschodí domov
* **Modul 2 - KOTOLŇA** je monitorovací a riadiaci, ktorý okrem snímania teplôt ovláda aj nezávisle na kúrení v domácnosti ventily radiátorov - solenoidy, napríklad prostredníctvom relé. Riadenie je nastavené manuálne - natvrdo ZAP/VYP pre každý solenoid, alebo automaticky na základe nameranej teploty z riadiaceho teplomera (navolí používateľ) z modulu 2. 
# MODUL 1 - BYT - Vývody - Arduino Mega 2560
| Zbernica | Vývod |
| --- | --- |
| I2C | SDA, SCL (Hardware) |
| Digital (DHT22) | D2 |
| Digital (DHT22) | D3 |
| OneWire A | D5 |
| OneWire B | D6 |
| OneWire C | D7 |
| OneWire D | D8 |

# MODUL 1 - BYT - Senzory a zbernice
| Číslo | Miesto | Typ | Záznam | Zbernica |
| --- | --- | --- | --- | --- |
| 1 | Obývačka | SHT21 | Teplota + vlhkosť | I2C |
| 2 | Obývačka | BME280 | Teplota + vlhkosť | I2C |
| 3 | Kúpelňa | DHT22 | Teplota + vlhkosť | Digital |
| 4 | Kúpelňa | DHT22 | Teplota + vlhkosť | Digital |
| 5 | Šatník | DS18B20 | Teplota | OneWire A |
| 6 | Rodičovská izba | DS18B20 | Teplota | OneWire A |
| 7 | Detská izba | DS18B20 | Teplota | OneWire B |
| 8 | Chodba | DS18B20 | Teplota | OneWire B |
| 9 | Predsieň | DS18B20 | Teplota | OneWire B |
| 10 | Kuchyňa | DS18B20 | Teplota | OneWire B |
| 11 | JZ - Atrium| DS18B20-X | Teplota | OneWire C |
| 12 | SV - Ulica | DS18B20-X | Teplota | OneWire D |

# MODUL 1 - BYT - Schéma zapojenia
![MODUL 1 - BYT - Schéma zapojenia pre Arduino](https://raw.githubusercontent.com/martinius96/Monitor-teploty-vlhkosti-kurenie/master/SCHEMA_BYT_MODUL_1.png)



# MODUL 2 - KOTOLŇA - Vývody - Arduino Mega 2560
| Zbernica | Vývod |
| --- | --- |
| OneWire A | D22 |
| OneWire B | D23 |
| OneWire C | D24 |
| OneWire D | D25 |
| OneWire E | D26 |
| OneWire F | D27 |
| OneWire G | D28 |
| OUT1 | D29 |
| OUT2 | D30 |
| OUT3 | D31 |
| OUT4 | D32 |
| OUT5 | D33 |
| OUT6 | D34 |
| OUT7 | D35 |
| OUT8 | D36 |
| IN1 | D37 |
| IN2 | D38 |
| IN3 | D39 |
| IN4 | D40 |
| IN5 | D41 |
| IN6 | D42 |
| IN7 | D43 |
| IN8 | D44 |

# MODUL 2 - KOTOLŇA - Senzory a zbernice
| Číslo | Miesto | Typ | Záznam | Zbernica |
| --- | --- | --- | --- | --- |
| 1 | BYT #1 IN | DS18B20-X | Teplota | OneWire A |
| 2 | BYT #1 OUT | DS18B20-X | Teplota | OneWire A |
| 3 | BYT #2 IN | DS18B20-X| Teplota | OneWire B |
| 4 | BYT #2 OUT | DS18B20-X | Teplota | OneWire B |
| 5 | BYT #3 IN | DS18B20-X | Teplota | OneWire C |
| 6 | BYT #3 OUT | DS18B20-X | Teplota | OneWire C |
| 7 | BYT #4 IN | DS18B20-X | Teplota | OneWire D |
| 8 | BYT #4 OUT | DS18B20-X | Teplota | OneWire D |
| 9 | BYT #5 IN | DS18B20-X | Teplota | OneWire E |
| 10 | BYT #5 OUT | DS18B20-X | Teplota | OneWire E |
| 11 | BYT #6 IN | DS18B20-X | Teplota | OneWire F |
| 12 | BYT #6 OUT | DS18B20-X | Teplota | OneWire F |
| 13 | KOMIN 1 | DS18B20-X | Teplota | OneWire G |
| 14 | KOMIN 2 | DS18B20-X | Teplota | OneWire G |
| 15 | Bojler VRCH | DS18B20-X | Teplota | OneWire G |
| 16 | Bojler STRED | DS18B20-X | Teplota | OneWire G |
| 17 | Termostat byt #1 | Digitálny vstup | IN1 | Digital |
| 18 | Termostat byt #2 | Digitálny vstup | IN2 | Digital |
| 19 | Termostat byt #3 | Digitálny vstup | IN3 | Digital |
| 20 | Termostat byt #4 | Digitálny vstup | IN4 | Digital |
| 21 | Termostat byt #5 | Digitálny vstup | IN5 | Digital |
| 22 | Termostat byt #6 | Digitálny vstup | IN6 | Digital |
| 23 | Termostat byt #7 | Digitálny vstup | IN7 | Digital |
| 24 | Termostat byt #8 | Digitálny vstup | IN8 | Digital |
| 25 | Čerpadlo byt #1 | Digitálny výstup | OUT1 | Digital |
| 26 | Čerpadlo byt #2 | Digitálny výstup | OUT2 | Digital |
| 27 | Čerpadlo byt #3 | Digitálny výstup | OUT3 | Digital |
| 28 | Čerpadlo byt #4 | Digitálny výstup | OUT4 | Digital |
| 29 | Čerpadlo byt #5 | Digitálny výstup | OUT5 | Digital |
| 30 | Čerpadlo byt #6 | Digitálny výstup | OUT6 | Digital |
| 31 | Čerpadlo byt #7 | Digitálny výstup | OUT7 | Digital |
| 32 | Čerpadlo byt #8 | Digitálny výstup | OUT8 | Digital |

# MODUL 2 - KOTOLŇA - Schéma zapojenia
![MODUL 2 - KOTOLŇA - Schéma zapojenia pre Arduino](https://raw.githubusercontent.com/martinius96/Monitor-teploty-vlhkosti-kurenie/master/SCHEMA_KOTOLNA_MODUL_2.png)
