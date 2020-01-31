# Monitor teplôt, vlhkostí, ovládanie ventilov - dokumentácia
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
| Číslo senzora | Miesto | Typ | Záznam | Zbernica |
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

# MODUL 2 - KOTOLŇA - Senzory a zbernice
| Číslo senzora | Miesto | Typ | Záznam | Zbernica |
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

# MODUL 2 - KOTOLŇA - Schéma zapojenia
![MODUL 2 - KOTOLŇA - Schéma zapojenia pre Arduino](https://raw.githubusercontent.com/martinius96/Monitor-teploty-vlhkosti-kurenie/master/SCHEMA_KOTOLNA_MODUL_2.png)
