===============================================================================
NÁVOD NA POUITÍ:
===============================================================================

1) Zkopírujte Váš .zip archiv pøipravenı na odevzdání do WIS do této sloky.

2) Hodnotu svého loginu vlote do promìnné LOGIN ve skriptu test.bat.

3) Spuste test projektu voláním skriptu test.bat.

===============================================================================
VİSTUPY:
===============================================================================

stdout - Struènı pøehled prùbìhu a vısledku testu.

work/ - Sloka obsahující pracovní soubory vytvoøené v prùbìhu testu.

work/log/ - Sloka obsahující podrobnosti o prùbìhu jednotlivıch krokù testu.

work/chk_sim_output.txt - Podrobnı pøehled vısledku testu.

POZN1: Test nekontroluje obsah dokumentace, ale pouze její existenci.
POZN2: Test nekontroluje funkènost odevzdaného accterm.bin na FITkitu, pouze
       jeho existenci. Je proto vhodné provést testy na FITkitu samostatnì.
POZN3: Vısledky testu jsou pouze orientaèní a mohou se lišit od koneèného
       hodnocení projektu. V zásadì ale, pokud test objeví nìkterı nedostatek, 
       je vysoce pravdìpodobné, e se tento nedostatek objeví i pøi hodnocení.


===============================================================================
CO TEST.BAT DÌLÁ:
===============================================================================

1) Nejprve si rozbalí vstupní .zip archiv a zkontroluje, zda obsahuje všechny
   poadované soubory ve správnıch slokách.

2) Provede zjednodušenou syntézu zdrojového souboru na ovìøení jeho korektnosti.

3) Spustí simulaci projektu v prostøedí ISIM na dvou sadách kódù. Jednoduchá
   sada testuje pøíjem správnıch kódù a odmítnutí úplnì nesprávnıch. Pokroèilá
   sada testuje odmítnutí kódù, které jsou velice podobné tìm správnım.

4) Provede ohodnocení dosaenıch vısledkù, tj. porovná odpovìdi automatu v log
   souborech získanıch v kroku (3) se vzorovımi odpovìïmi. Vısledky tohoto
   porovnání a nalezené špatnì vyhodnocené kódy jsou uloeny do souboru
   work/chk_sim_output.txt. Zkrácenı vıpis je pak vypsán na stdout.
