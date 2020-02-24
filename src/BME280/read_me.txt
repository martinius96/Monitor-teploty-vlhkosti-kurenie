Bezpecnost pocitacovych systemov - PS, 6. tyzden
------------------------------------------------
2019-03-18, v.1.0, MD KEMT FEI TUKE

Testovanie RNG

Digitálne podpisovanie dát a certifikáty 

Pouzite referencie
[1] Drutarovský, M.: Kryptografia pre vstavané kryptograficke systemy. TUKE, 2017

Ciel: 
-----
- vybrane testy RNG
- ovladanie OpenSSL z prikazoveho riadku
- test RNG instrukcii v Intel CPU
- princip digitalneho podpisu (CrypTool)
- generovanie certifikátov pomocou OpenSSL a transformacia formatov
 
 

Vybrane testy RNG + ovladanie OpenSSL z prikazoveho riadku
----------------------------------------------------------
OpenSSl obsahuje aj kryptograficky bezpecny PRNG (v starsich verziach vyuzival hashovaciu funkciu SHA1, 
pozri [1, str.82-85]. OpenSSL je mozne ovladat aj z prikazoveho riadku (takze OpenSSL je aj spustitelna 
aplikacia, ktora vyuziva OpenSSL kniznicu), co vyuzijeme v nasledujucom priklade.

Uloha 1
-------
Pomocou PRNG, ktory je implementovany v knižnici OpenSSL vygenerujeme 10 000 bajtov a overime, že takto 
vygenerovanu postupnost bajtovnie je možné komprimovt programom WinRAR (jeden z najlepsich kompresnych
programov pre vseobecne pouzitie). Tato podmienka musi byt pre kvalitny PRNG (a tiez TRNG) vzdy splnena, 
kedze idealna nahodna postupnost je nekomprimovatelna.

Riešenie:
V rozhraní OpenSSL máme z príkazového riadku k dispozícii funkciu rand, ktorej volby zobarzime prikazom 
openssl
openssl> rand –help

Pomocou prikazu*
openssl rand –out prng.bin 10000

vygenerujeme pozadovanu postupnost prng.bin s velkostou 10 000 bajtov. Po kompresii programom WinRAR 
s maximalnou urovnou kompresie dostaneme velkost „komprimovaného“ suboru cca 10 146 bajtov, co potvrdzuje 
skutocnost, ze vystup kvalitneho PRNG (aj napriek tomu, že vygenerovaná postupnost ma malu neurcitost) 
nie je mozne univerzalnymi komprimacnymi programami zmenzit.

*Help k roznym verziam OpenSSL je mozne najst napr. aj na stranke (vsimnite si mnozstvo dostunych funkcii):
https://www.openssl.org/docs/man1.1.1/man1/openssl-req.html


Uloha 2
-------
Na predchadzajucej prednaske sme prebrali zakladne statisticke testy FIPS 140-2 na testovanie vyattupu RNG 
(monobit test, poker test, runs test, long runs test). Vysvetlite jednotlive FIPS 140-2 testy a s vyuzitim 
implementacie tychto testov v jazyku C:

FIPS\FIPS140.C

otestujte prvych 20000 bitov postupnosti prng.bin z predchadzajucej ulohy.

Na preklad zdrojoveho kodu v jazyku C vyuzite GCC prekladac. Na zaklade zdrojoveho kodu zistite s akymi 
parametrami je potrebne prelozeny program zavolat, aby vypisal aj vystupne statisticke informacie.

Ako sa zmenia vysledky testovania ak namiesto RNG postupnosti pouzijete nejaky textovy subor?


Test RNG instrukcii v Intel CPU
-------------------------------

Uloha 3
-------
Adresar:

INTEL_RAND_TEST

obsahuje dva C programy, ktore umoznuju otestovat dostupnost RDRAND a RDSEED na testovanej CPU.
Pomocou GCC prekladaca otestujte dostupnost uvedenych instrukcii na Vasom pocitaci.
Ak je instrukcia RDRAND dostupna (niektore CPU maju implementovanu len instrukciu RDRAND, pripadne ziadnu 
z nich), vycitajte 20000 bitov a otetsujte ich statisticku kvalitu.



Uloha 4
-------
Nastroj CrypTool obsahuje v polozke
Digital Signatures/PKI ->
viacero moznosti testovania digitalnych podpisov vratane vizualizacie.
Otestujte pomocou CrypTool nastroja jednotlive kroky algoritmu DSA.


Generovanie certifikátov pomocou OpenSSL a transformacia formatov
-----------------------------------------------------------------
Nastroj OpenSSL umoznuje (okrem monohych dalsich funkcii) generovat sifrovacie kluce pre 
PKI infrastrukturu, generovat certifikaty a tiez realizovat ich konverziu. 
V dalsich cviceniach budeme OpenSSL pouzivat napre. na generovanie certifikatov pre 
sifrovanu komunikaciu medzi klientom a serverom. 

Uloha 5
-------
Adresar:

OPENSSL_PRIKAZY

obsahuje dokumentaciu s opisom prikazov, ktore mozeme pri generovani a transformacii certifikatov 
vyuzit. Prestudujte zakladne prikazy a vygenerujte pomocou nich 1024-bitove RSA kluce a certifikaty,
ktore v dalsich cviceniach vyuzijeme na sifrovanu komunikaciu klien server s vyuzitim TLS.

Pomocka: 
Archiv gen_RSA_certifikaty.zip obsahuje vhidne prikazy, ktore mozete pouzit.



 


