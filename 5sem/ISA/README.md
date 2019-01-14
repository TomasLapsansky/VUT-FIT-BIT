Aplikacia myripsniffer nacuva na rozhrani na portoch 520 a 521 a vypisuje detaily o prevadzke
Priklad spustenia:
./myripsniffer -i <env>
env	interface, ktory je mozny najst napr v ifconfig

Aplikacia myripresponse zasiela multicastom vygenerovany ripng response
Priklad spustenia:
./myripresponse -i <env> -r <IPv6>/[16-128] {-n <IPv6>} {-m [0-16]} {-t [0-65535]}
kde majú jednotlivé parametre nasledujúci význam
* -i: udává rozhranie, z ktorého má byť paket odoslaný
* -r: IP adresa podvrhávanej siete a jej dĺžka prefixu * -m: RIP metrika (tzv. počet hopov), implicitne 1 * -n: adresa next-hopu, implicitne ::
* -t: hodnota router tagu, implicitne 0

