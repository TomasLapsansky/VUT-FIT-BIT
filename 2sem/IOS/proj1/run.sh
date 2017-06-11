#!/usr/bin/env bash

START=$(date +%s)

help() {
printf \\n%s\\n "Spusteni testu:
	$0		Spusti skript dirgraph pomoci shellu bash
	$0 dash		Spusti skript dirgraph pomoci shellu dash
	$0 ksh		Spusti skript dirgraph pomoci shellu ksh
	$0 sh		Spusti skript dirgraph pomoci shellu sh
	$0 xxx		dtto, pokud xxx neexistuje vypise chybu a help

	$0 -h		Vypise tento help
	$0 -c		Uklid souboru vytvorenych v prubehu testovani
				maze soubory *-my a *-error	
Jak to funguje:
	Pro kazde JMENO testu (pouzivaji se cisla) vytvori nazvy souboru:
		testJMENO-out
		testJMENO-my
		testJMENO-error

	testJMENO-out existuje => skript ma vracet 0 a nove vytvoreny soubor
		testJMENO-my ma byt identicky
	testJMENO-out neexistuje => skript ma vracet nenulovou hodnotu a 
		zapsat neco na stderr, ktere se uklada do testJMENO-error"
}

file='dirgraph'


# Output settings
TEXT_BOLD=`tput bold`
TEXT_GREEN=`tput setaf 2`
TEXT_RED=`tput setaf 1`
TEXT_RESET=`tput sgr0`
TEXT_BLUE=`tput setaf 4`
TEXT_BROWN=`tput setaf 3`

# IO file names
function errorFileName () {
    printf "test${1}-error"
}
function inFileName () {
    printf "test${1}-in"
}
function outFileName () {
    printf "test${1}-out"
}
function myFileName () {
    printf "test${1}-my"
}

# Test initialization
function initTest () {
    testNumber=$1
    testError=$(errorFileName $testNumber)
    testOut=$(outFileName $testNumber)
    testMy=$(myFileName $testNumber)
}


function green() {
    printf %s "${TEXT_GREEN}$1${TEXT_RESET}"
}

function red() {
    printf %s "${TEXT_RED}$1${TEXT_RESET}"
}


function isOk () {
    local testNumber=$1
    local   testExit=$2
    local  testError=$3

    printf "Test %03d	.." "$1"

    if [ -e ${testOut} ] 
    then
    
        `$diff -q ${testOut} ${testMy} > /dev/null`
        local diffResult=$?
        printf %s "isOK   ExitCode: "
        [ $testExit -eq 0 ] && green $testExit || red $testExit
        printf %s\\n ",       output: $([ $diffResult -eq 0 ] && green 'not diff' || red 'diff')"

        err=1
        [ $testExit == 0 ] && [ $diffResult == 0 ] && err=0
    
    else

        printf %s "isFail ExitCode: " 
        [ $testExit -eq 0 ] && red $testExit || green $testExit
        printf %s\\n ", error output: $([ -s $testError ] && green "found" || red "not found")"
        
        err=1
        [ $testExit != 0 ] && [ -s $testError ] && err=0 # True, if <FILE> exists and has size bigger than 0 (not empty).

	[ $err -eq 0 ] && printf "$TEXT_BROWN" && /bin/cat $testError
    fi

    [ $err -eq 0 ] && green "ok" || red "fail"

}



test() {
    no=$1
    par="$2"

    initTest ${no}

    `${spoustec} ./${file} $par > ${testMy} 2> ${testError}`
    isOk $testNumber $? $testError
    
    printf \\t%s\\n\\n "${TEXT_BLUE}$info${TEXT_RESET}"

}


file_exists() {
	if [ -e "$1" ]
	then
    		printf "${TEXT_BOLD}Testing file ${1}${TEXT_RESET}\n"
	else
    		printf "${TEXT_RED}Cannot run test without file ${1}.${TEXT_RESET}\n"
    		exit 1
	fi
}


# Cleaning
if [ "$1" == "-h" ]
then
	help
	exit 0
fi

# Cleaning
if [ "$1" == "-c" ]
then
	ls | grep 'test.\+\-\(my\|error\)$' | xargs -d "\n" rm
	exit 0
fi

if [ "$#" -eq 1 ]
then
	spoustec="$1"
else
	spoustec=bash
fi

spoustec=$(which $spoustec)
if [ $? -ne 0 ]
then
	printf "${TEXT_RED}Spatny parametr ${1}.${TEXT_RESET}\n"
	help
	exit 1
fi

diff=$(which diff)

file_exists "$file"

printf %s\\n "Zakladni testy funkcnosti (spousteno pomoci $spoustec)"

info="Zadany 2 parametry bez -i a -n."
test 1 "a b"

info="Zadan neexistujici adresar."
test 2 neexistuje

#info="Zadano 3x -i."
#test 3 "-i -i -i"

#info="Zadano 3x -n."
#test 4 "-n -n -n"

info="Prilis mnoho parametru -i a -n a a."
test 5 "-i a -n a a"

info="Odkaz na prazdny adresar."
test 6 ./test_data/empty

info="Odkaz na prazdny adresar s -n na zacatku."
test 7 "-n ./test_data/empty"

info="Odkaz na prazdny adresar s -n na konci."
test 8 "./test_data/empty -n"

info="Test prvnich 3 velikosti."
test 9 "./test_data/velikosti"

info="To same se zbytecnym parametrem -n."
test 10 "-n ./test_data/velikosti"

info="Test s jedinym souborem a lomitkem na konci cesty."
test 11 "./test_data/one/"

info="Test prilis mnoho souboru."
test 12 "./test_data/nas_mnogo"

info="Test prilis mnoho souboru s -n na zacatku"
test 13 "-n ./test_data/nas_mnogo"

info="Test prilis mnoho souboru s -n na konci"
test 14 "./test_data/nas_mnogo -n"

#info="Test vice formatu jak 10."
#test 15 "./test_data/formaty"

#info="Test vice formatu jak 10 s -n."
#test 16 "-n ./test_data/formaty"

#info="Test vice formatu jak 10 s -n a -i '^Z80_Compo_6$'.
#	Sance ze to selze kvuli jinemu poradi vystupu,
#	protoze je nedefinovany pro stejne hodnoty"
#test 17 '-n -i ^Z80_Compo_6$ ./test_data/formaty'

info="Jiny test na -n a -i"
test 18 '-i ^10 -n ./test_data/nas_mnogo'

info="Tezky test s hnusnymi jmeny souboru."
test 19 "./test_data/hardcore"

info="Tezky test s hnusnymi jmeny souboru a -i, plus nepokryti cesty."
test 20 "-i ^test_data$ ./test_data/hardcore"

#info="Cely adresar /test_data"
#test 21 "./test_data"

#info="2x -n"
#test 22 "-n -n"

#info="2x -i"
#test 23 "-i aaa -i bbb"

info="Snadny vyraz -i 1"
test 24 "-i 1 ./test_data/nas_mnogo"

info="Snadny vyraz -i 1$"
test 25 "-i 1$ ./test_data/nas_mnogo"

info="Snadny vyraz -i ^9"
test 26 "-i ^9 ./test_data/nas_mnogo"

info="Slozeny vyraz -i (^9|1$)"
test 27 "-i (^9|1$) ./test_data/nas_mnogo"

info="Slozeny vyraz -i (^9|[16]$)"
test 28 "-i (^9|[16]$) ./test_data/nas_mnogo"

info="Bez vyrazu za -i"
test 29 "-i"

info="Nefunguji odkazy (jen jsem zrusil cestu), ocekavam ze skript zjisti ze selhal"
OLD="$PATH"
PATH=""
test 30 "./test_data/one"
PATH="$OLD"

# 31 test ---------------------------
info="Skript prekopirovan do ./test_data/nas_mnogo a spusten bez parametru
	nekolik varovani: 
		1. doufam ze mate skript do 10k
		2. ze je to spustitelny soubor
	Jinak si vysledek upravte. .)"
cp dirgraph ./test_data/nas_mnogo/
cd ./test_data/nas_mnogo/


initTest "31"
testError="./../../$testError"
testOut="./../../$testOut"
testMy="./../../$testMy"

zbytek=$(tail -n +2 $testOut)
printf %s\\n%s\\n "Root directory: $PWD" "$zbytek" > $testOut

`${spoustec} ./${file} > ${testMy} 2> ${testError}`
isOk $testNumber $? $testError   
printf \\t%s\\n\\n "${TEXT_BLUE}$info${TEXT_RESET}"
rm dirgraph
cd ./../..
# ---------------------------

END=$(date +%s)
printf "Test trval vterin: $(( $END - $START ))\n"
