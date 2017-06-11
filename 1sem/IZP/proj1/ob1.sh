#!/bin/bash
# Skript pro obhajobu prvniho projeku do IZP - studenti
# Autor: Ing. Petr Veigend
# Rok: 2016
# Verze: 1.0

login=false

if [ "$#" -eq 1 ]; then
    cp pattern.txt $1
    cd $1
    $login = true     
fi

#preklad programu
gcc -std=c99 -Wall -Wextra -Werror proj1.c -o proj1
	
if [ ! -f proj1 ]; then 
    echo "Chyba pri prekladu!"
    exit 1
fi

# odstraneni vystupniho souboru
if [ -f output.txt ]; then
    rm output.txt
fi

# provedeni testu
echo "Hello, world! Ahoj svete!" | ./proj1 >> output.txt
echo "Ahoj" | ./proj1 >> output.txt
echo "Hello, world! Ahoj svete!" | ./proj1 -s 14 -n 5 >> output.txt
echo "Hello" | ./proj1 -x >> output.txt
printf "\xff\x00" | ./proj1 -x >> output.txt
printf "Ahoj\0Slovo\nXY\n" | ./proj1 -S 3 >> output.txt
echo "48656c6c0a" | ./proj1 -r >> output.txt
echo "00ff00ff" | ./proj1 -r | ./proj1 -x >> output.txt
echo "" >> output.txt

#porovnani vysledku
diff output.txt pattern.txt

if [ -f proj1 ]; then 
    rm proj1
fi

if [ $login ]; then
    cd ..
fi
    

