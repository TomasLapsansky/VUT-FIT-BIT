#!/bin/bash

# testuje spravnou sekvenci cislovani akci a format vystupu (pouze syntax)
# bez zaruky, muze obsahovat chyby

outf="proj2.out"
[ $# -eq 1 ] && outf="$1" && shift
[ $# -ne 0 ] && echo "Spatny pocet parametru, ocekavam zadny pak se hleda soubor proj2.out a nebo jmeno testovaneho souboru." >&2 && exit 1
[ ! -f "${outf}" ] && echo "Soubor '$outf' nenalezen." >&2 && exit 1

A_MAX=$(cat "${outf}" | grep started | cut -d: -f2 | grep A | sed "s#^[^0-9]*\([0-9]*\).*#\1#g" | sort -rn | head -n1)
C_MAX=$(cat "${outf}" | grep started | cut -d: -f2 | grep C | sed "s#^[^0-9]*\([0-9]*\).*#\1#g" | sort -rn | head -n1)

cat "${outf}" | sed -e "s#[\t ]##g" -e "s#:# #g" | awk '
function Print_Error_Neprovedl(kdo,text) { print kdo, "jeste neprovedl akci \"", text, "\"\r\t\t\t\t\t\t\t =>", $0 >"/dev/fd/2"; }
function Print_Error(text) { print text, "\r\t\t\t\t\t\t\t =>", $0 >"/dev/fd/2"; }
BEGIN {
A_Center=0
C_Center=0
A_Sum_Leave=0
C_Sum_Leave=0
A_Sum_Finished=0
C_Sum_Finished=0

A_Out=0
Do_Not_Enter=0

}
{
	id=$2
	typ=substr(id,1,1)
	akce=$3

	if ( akce == "started" ) 
	{
		Started[id]++
		if ( Started[id] > 1 ) Print_Error("Duplicitni akce \"started\"");
	} else if ( akce == "enter" ) 
	{
		Enter[id]++
		if ( Started[id] == "" ) Print_Error_Neprovedl(id,"started");
		else if ( Do_Not_Enter ) Print_Error( "Porusena podminka nevstupovani po \"trying to leave\"");
		else if ( Enter[id] > 1 ) Print_Error("Duplicitni akce \"enter\"");
		else if ( typ == "A" ) {
			A_Center+=3
		} else {
			C_Center++
			if ( A_Sum_Leave != '$A_MAX' && C_Center > A_Center ) Print_Error("V centru jsou nehlidane deti");
		}
	} 
	else if ( akce == "tryingtoleave" ) 
	{
		Trying[id]++
		if ( Enter[id] == "" ) Print_Error_Neprovedl( id,"enter");
		else if ( Trying[id] > 1 ) Print_Error("Duplicitni akce \"trying to leave\"");
		else if ( typ == "A" ) {
			Do_Not_Enter++
			A_Out+=3
		}
	}
	else if ( akce == "waiting" ) 
	{
		Waiting[id]++
		if ( 3*$4 != A_Center )	Print_Error("Nesouhlasi pocet Adult v centru")
		if ( $5 != C_Center )  Print_Error("Nesouhlasi pocet Child v centru")

		if ( Waiting[id] > 1 ) Print_Error("Duplicitni akce \"waiting\"");
		else if ( typ == "A" ) {
			Who_Waited[id]++
			Do_Not_Enter--
			if ( A_Center > 3+C_Center ) Print_Error("Adult ceka v centru a nemusi");	
		} else {
			if ( Started[id] == "" ) Print_Error_Neprovedl( id,"started");			
			if ( A_Center-A_Out > C_Center ) print id,"ceka pred centrem a nemusi (", A_Center-A_Out, ">" , C_Center ,")\t\t =>", $0 >"/dev/fd/2"; 
			if ( '$A_MAX' == A_Sum_Leave ) Print_Error("Child ceka pred centrem a adult uz neprijde");
		}
	} 
	else if ( akce == "leave" ) 
	{
		Leave[id]++
		if ( Trying[id] == "" ) Print_Error_Neprovedl(id,"trying to leave");
		else if ( Leave[id] > 1 ) Print_Error("Duplicitni akce \"leave\"");
		else if ( typ == "A" ) {
			A_Sum_Leave++
			A_Center-=3
			A_Out-=3
			if ( C_Center > A_Center ) Print_Error("V centru jsou nehlidane deti");
			if ( Who_Waited[id] == 0 ) Do_Not_Enter--
		} else {
			C_Sum_Leave++
			C_Center--
		}
	} 
	else if ( akce == "finished" ) 
	{
		if ( Leave[id] == "" ) Print_Error_Neprovedl(id,"leave");
		if ( A_Sum_Leave != '$A_MAX' ) Print_Error( "Nektere adult jeste neopustili centrum"); 
		if ( C_Sum_Leave != '$C_MAX' ) Print_Error( "Nektere child jeste neopustili centrum");  
		if ( typ == "A" ) A_Sum_Finished++; else C_Sum_Finished++;

	} else
	{
		print "wtf akce =",akce,"?\t =>", $0 >"/dev/fd/2"; 
	}
}
END {
	if ( '$A_MAX' != A_Sum_Finished ) { print "Rozdilny pocet startujicich a ukoncenych adult\n" >"/dev/fd/2"; }
	if ( '$C_MAX' != C_Sum_Finished ) { print "Rozdilny pocet startujicich a ukoncenych child\n" >"/dev/fd/2"; }
}'

tr -d " \t" < "${outf}" > $$

# test cislovani akci
# tiskne radky s chybnym cislovanim
cat $$ | awk -F":" ' { c++; if (c != $1) print NR, "\t => ", $1, " : chyba v cislovani akci"; } '

declare -a lines
 lines[0]='^[1-9][0-9]*:C[1-9][0-9]*:started$'
 lines[1]='^[1-9][0-9]*:C[1-9][0-9]*:waiting:[0-9][0-9]*:[0-9][0-9]*$'
 lines[2]='^[1-9][0-9]*:C[1-9][0-9]*:enter$'
 lines[3]='^[1-9][0-9]*:C[1-9][0-9]*:tryingtoleave$'
 lines[4]='^[1-9][0-9]*:C[1-9][0-9]*:leave$'
 lines[5]='^[1-9][0-9]*:C[1-9][0-9]*:finished$'
 lines[6]='^[1-9][0-9]*:A[1-9][0-9]*:started$'
 lines[7]='^[1-9][0-9]*:A[1-9][0-9]*:enter$'
 lines[8]='^[1-9][0-9]*:A[1-9][0-9]*:tryingtoleave$'
 lines[9]='^[1-9][0-9]*:A[1-9][0-9]*:waiting:[0-9][0-9]*:[0-9][0-9]*$'
lines[10]='^[1-9][0-9]*:A[1-9][0-9]*:leave$' 
lines[11]='^[1-9][0-9]*:A[1-9][0-9]*:finished$'

# kontrola sytaxe vystupu
# vytiskne radky, ktere neodpovidaji formatu vytupu
echo "=== radky, ktere neodpovidaji formatu vystupu"

for i in `seq 0 11`
do
    echo "/${lines[$i]}/d" >> $$-sed
done

sed -f $$-sed $$

# kontrola chybejicich vystupu
# tiskne informaci, ktery text ve vystupu chybi
echo "=== chybejici vystupy"
echo ">> chybejici waiting nemusi nutne znamenat chybu, za urcitych okolnosti nemusi ve vystupu byt <<"

for i in `seq 0 11`
do
    cat $$ | grep "${lines[$i]}" >/dev/null || echo "${lines[$i]}"
done


rm $$*
