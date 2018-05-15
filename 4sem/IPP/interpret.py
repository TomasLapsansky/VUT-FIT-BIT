#!/usr/bin/python
# -*- coding: UTF-8 -*-

# Tomas Lapsansky
# FIT VUT Brno (xlapsa00)
# 15.4.2018
# Interpret

import sys
import getopt
import re   # for ElementTree texts
from xml.etree import ElementTree

i = 0


class Core:

    def __init__(self):

        self.Frame = {'GF': {}}
        #   self.GF = {}
        #   self.LF  will be created in runtime when PUSHFRAME
        #   self.TF  will be created in runtime when CREATEFRAME
        self.LF_Stack = []
        self.Call_Stack = []
        self.Data_Stack = []
        self.labels = {}

    def var_analyzer(self, var_xml, exit_bool):
        """
        Analyze correctness of variable
        :param var_xml: Variable
        :param exit_bool: Determine if program should be exited if variable doesn't exists or not
        :return: (Frame, Variable name)
        """
        var = var_xml['Text']
        if len(var) < 4 or var[2] != '@':   # This is not variables
            exit_error("Bad variables!", 53)

        if var[0:2] != "GF" and var[0:2] != "LF" and var[0:2] != "TF":
            exit_error("Bad variable declaration", 53)

        frame = var[0:2]
        var_name = var[3:]

        if frame not in self.Frame:
            exit_error("Unexisting frame", 55)

        if exit_bool:   # Testing existence of Variables
            if var_name in self.Frame[frame]:
                pass
            else:
                exit_error("Variable doesn't exists", 54)

        return frame, var_name

    @staticmethod
    def symbol_join(symb):
        if symb['Attrib']['type'] != 'int' and symb['Attrib']['type'] != 'bool'\
                and symb['Attrib']['type'] != 'string':
            exit_error("Wrong type", 53)
        else:
            if symb['Text'] is None:
                return symb['Attrib']['type'] + '@'
            else:

                for c in str(symb['Text']):     # whitespace checker
                    if c.isspace():
                        exit_error("Whitespace in string", 52)

                return symb['Attrib']['type'] + '@' + str(symb['Text'])

    def symb_var_split(self, symbvar):
        """
        Determines if a symbvar is symbol or var, and split it to the type and value
        :param symbvar: Variable/Symbol
        :return: Type, Value
        """
        if symbvar['Attrib']['type'] == 'var':
            frame, name = self.var_analyzer(symbvar, True)
            var = self.Frame[frame][name]

            if var == '':
                "Uninitialized var"
                return "", ""
        else:
            var = self.symbol_join(symbvar)

        if var[0:3] == 'int':  # int value
            if var[3] == '@':
                v_type = 'int'
                value = var[4:]
                return v_type, value
            else:
                exit_error("Value error", 53)

        elif var[0:4] == 'bool':
            if var[4] == '@':
                v_type = 'bool'
                value = var[5:]
                return v_type, value
            else:
                exit_error("Value error", 53)

        elif var[0:6] == 'string':
            if var[6] == '@':
                v_type = 'string'
                value = var[7:]
                return v_type, value
            else:
                exit_error("Value error", 53)
        else:
            exit_error("Value error", 53)

        return None, None   # for correctness

    def label_analysis(self, xml_list):
        """
        Adding label with his index to the dictionary
        :param xml_list:
        """
        index = 0
        while index < len(xml_list):
            if xml_list[index]['Tag'] == 'instruction' and xml_list[index]['Attrib']['opcode'] == 'LABEL':
                if xml_list[index+1]['Tag'] == 'arg1':
                    if xml_list[index+1]['Text'] in self.labels:
                        exit_error("Duplicated labels", 52)
                    else:
                        self.labels[xml_list[index+1]['Text']] = index

            index += 1

    def move(self, var, symb):
        """
        Zkopíruje hodnotu ⟨symb⟩ do ⟨var⟩. Např. MOVE LF@par GF@var provede zkopírování hodnoty proměnné
        var v globálním rámci do proměnné par v lokálním rámci.
        :param var: Variable
        :param symb: Variable/Symbol
        """
        if var['Attrib']['type'] != 'var':
            exit_error("Bad variable", 53)

        if symb['Attrib']['type'] == 'var':  # Moving var to var
            frame1, var1 = self.var_analyzer(var, True)
            frame2, var2 = self.var_analyzer(symb, True)

            self.Frame[frame1][var1] = self.Frame[frame2][var2]

        else:
            frame, name = self.var_analyzer(var, True)
            self.Frame[frame][name] = self.symbol_join(symb)  # symbol_join calls error

    def createframe(self):
        """
        Vytvoří nový dočasný rámec a zahodí případný obsah původního dočasného rámce.
        """
        self.Frame['TF'] = {}
        self.Frame['TF'].clear()

    def pushframe(self):
        """
        Přesuň TF na zásobník rámců. Rámec bude k dispozici přes LF a překryje původní
        rámce na zásobníku rámců. TF bude po provedení instrukce nedefinován a je třeba
        jej před dalším použitím vytvořit pomocí CREATEFRAME.
        Pokus o přístup k nedefinovanému rámci vede nahybu 55.
        """
        if 'TF' in self.Frame:    # If TF exists, DO
            self.LF_Stack.append(self.Frame['TF'])
            del self.Frame['TF']
            self.Frame['LF'] = self.LF_Stack[len(self.LF_Stack) - 1]
        else:
            exit_error("TF doesn't exists", 55)

    def popframe(self):
        """
        Přesuň vrcholový rámec LF ze zásobníku rámců do TF. Pokud žádný rámec v LF
        není k dispozici, dojde k chybě 55.
        """
        if len(self.LF_Stack) == 0:
            exit_error("Empty Local Frame stack", 55)

        self.Frame['TF'] = self.LF_Stack.pop()

        if len(self.LF_Stack) == 0:
            del(self.Frame['LF'])
        else:
            self.Frame['LF'] = self.LF_Stack[len(self.LF_Stack) - 1]

    def defvar(self, var):
        """
        Definuje proměnnou v určeném rámci dle ⟨var⟩. Tato proměnná je zatím
        neinicializovaná a bez určení typu, který bude určen až přiřazením nějaké hodnoty.
        :param var: Variable
        """
        if var['Attrib']['type'] != 'var':
            exit_error("Bad variable", 53)

        frame, var = self.var_analyzer(var, False)
        self.Frame[frame][var] = ''

    def call(self, label, index):
        """
        Uloží inkrementovanou aktuální pozici z interního čítače instrukcí do zásobníku volánía provede
        skok na zadané návěští (případnou přípravu rámce musí zajistit jiné instrukce).
        :param label: Label
        :param index: Index of Code
        :return: Line of Code
        """
        self.Call_Stack.append(index)  # neukladam inkrement, return nas vrati a automaticky skoci na zaciatok while

        # label validation
        if label[0:5] != "label" or label[5] != '@':
            exit_error("Bad label type", 55)

        if len(label) <= 7:
            exit_error("Bad label type", 55)

        if label[7:] in self.labels:
            return self.labels[label[6:]]
        else:
            exit_error("Calling of unexisting Label", 52)

    def f_return(self):
        """
        Vyjme pozici ze zásobníku volání a skočí na tuto pozici nastavením interního čítače instrukcí
        (úklid lokálních rámců musí zajistit jiné instrukce).
        :return: Line of Code from Call_Stack
        """
        if len(self.Call_Stack) == 0:
            exit_error("Illegal return", 56)

        return self.Call_Stack.pop()

    def pushs(self, symb):
        """
        Uloží hodnotu ⟨symb⟩ na datový zásobník.
        :param symb: Variable/Symbol
        """
        if symb['Attrib']['type'] == 'var':
            frame, var = self.var_analyzer(symb, True)
            self.Data_Stack.append(self.Frame[frame][var])
        else:
            self.Data_Stack.append(self.symbol_join(symb))

    def pops(self, var):
        """
        Není-li zásobník prázdný, vyjme z něj hodnotu a uloží ji do proměnné ⟨var⟩, jinak dojde k chybě 56.
        :param var:
        """
        frame, name = self.var_analyzer(var, True)

        if len(self.Data_Stack) == 0:
            exit_error("Calling pops on empty stack", 56)

        self.Frame[frame][name] = self.Data_Stack.pop()

    def add(self, var, symb1, symb2):
        """
        Sečte ⟨symb1⟩ a ⟨symb2⟩ (musí být typu int) a výslednou hodnotu téhož typu uloží do proměnné ⟨var ⟩.
        :param var: Variable
        :param symb1: int Variable/Symbol
        :param symb2: int Variable/Symbol
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != 'int' or v_type2 != 'int':
            exit_error("Wrong type of argument in ADD", 53)

        result = int(value1) + int(value2)

        self.Frame[frame][name] = "int@" + str(result)

    def sub(self, var, symb1, symb2):
        """
        Odečte ⟨symb2⟩ od ⟨symb1⟩ (musí být typu int) a výslednou hodnotu téhož typu uloží do proměnné ⟨var⟩.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != 'int' or v_type2 != 'int':
            exit_error("Wrong type of argument in SUB", 53)

        result = int(value1) - int(value2)

        self.Frame[frame][name] = "int@" + str(result)

    def mul(self, var, symb1, symb2):
        """
        Vynásobí ⟨symb1⟩ a ⟨symb2⟩ (musí být typu int) a výslednou
        hodnotu téhož typu uloží do proměnné ⟨var⟩.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != 'int' or v_type2 != 'int':
            exit_error("Wrong type of argument in MUL", 53)

        result = int(value1)*int(value2)

        self.Frame[frame][name] = "int@" + str(result)

    def idiv(self, var, symb1, symb2):
        """
        Celočíselně podělí celočíselnou hodnotu ze ⟨symb1⟩ druhou celočíselnou
        hodnotou ze ⟨symb2⟩ a výsledek typu int přiřadí do proměnné ⟨var⟩.
        Dělení nulou způsobí chybu 57.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != 'int' or v_type2 != 'int':
            exit_error("Wrong type of argument in IDIV", 53)

        if int(value2) == 0:
            exit_error("Zero division", 57)

        result = int(value1)/int(value2)

        self.Frame[frame][name] = "int@" + str(result)

    def lt(self, var, symb1, symb2):
        """
        Instrukce vyhodnotí relační operátor mezi ⟨symb1⟩ a ⟨symb2⟩ (stejného typu; int, bool nebo string)
        a do booleovské proměnné ⟨var⟩ zapíše false při neplatnost nebo true v případě
        platnosti odpovídající relace. Řetězce jsou porovnávány lexikograficky a false je menší než true.
        Pro výpočet neostrých nerovností
        lze použít AND/OR/NOT.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 == v_type2:

            if v_type1 == 'int':

                if int(value1) < int(value2):
                    self.Frame[frame][name] = "int@true"
                else:
                    self.Frame[frame][name] = "int@false"

            elif v_type1 == 'bool':

                if value1 == 'false' and value2 == 'true':
                    self.Frame[frame][name] = "int@true"
                else:
                    self.Frame[frame][name] = "int@false"

            elif v_type1 == 'string':

                if value1 < value2:
                    self.Frame[frame][name] = "int@true"
                else:
                    self.Frame[frame][name] = "int@false"

            else:
                exit_error("Bad variable types", 53)  # theoretically impossible

        else:
            exit_error("Incompatible parameters", 53)

    def gt(self, var, symb1, symb2):
        """
        Instrukce vyhodnotí relační operátor mezi ⟨symb1⟩ a ⟨symb2⟩ (stejného typu; int, bool nebo string)
        a do booleovské proměnné ⟨var⟩ zapíše false při neplatnost nebo true v případě
        platnosti odpovídající relace. Řetězce jsou porovnávány lexikograficky a false je menší než true.
        Pro výpočet neostrých nerovností
        lze použít AND/OR/NOT.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 == v_type2:

            if v_type1 == 'int':

                if int(value1) < int(value2):
                    self.Frame[frame][name] = "int@true"
                else:
                    self.Frame[frame][name] = "int@false"

            elif v_type1 == 'bool':

                if value1 == 'true' and value2 == 'false':
                    self.Frame[frame][name] = "int@true"
                else:
                    self.Frame[frame][name] = "int@false"

            elif v_type1 == 'string':

                if value1 > value2:
                    self.Frame[frame][name] = "int@true"
                else:
                    self.Frame[frame][name] = "int@false"

            else:
                exit_error("Bad variable types", 53)  # theoretically impossible

        else:
            exit_error("Incompatible parameters", 53)

    def eq(self, var, symb1, symb2):
        """
        Instrukce vyhodnotí relační operátor mezi ⟨symb1⟩ a ⟨symb2⟩ (stejného typu; int, bool nebo string)
        a do booleovské proměnné ⟨var⟩ zapíše false při neplatnost nebo true v případě platnosti
        odpovídající relace. Řetězce jsou porovnávány lexikograficky a false je menší než true.
        Pro výpočet neostrých nerovností lze použít AND/OR/NOT.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)  # HAVE TO SAY THAT EQ IS BEAUTIFUL :')

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 == v_type2:

            if value1 == value2:
                self.Frame[frame][name] = "int@true"
            else:
                self.Frame[frame][name] = "int@false"

        else:
            exit_error("Incompatible parameters", 53)

    def f_and(self, var, symb1, symb2):
        """
        Aplikuje konjunkci (logické A)/disjunkci (logické NEBO) na ⟨symb1⟩ a ⟨symb2⟩ nebo negaci na ⟨symb1⟩
        (NOT má pouze 2 operandy) a výsledek zapíše do ⟨var⟩ (všechny operandy jsou typu bool).
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 == 'bool' and v_type2 == 'bool':

            if value1 == 'true' and value2 == 'true':
                self.Frame[frame][name] = "int@true"
            else:
                self.Frame[frame][name] = "int@false"

        else:
            exit_error("Incompatible parameters", 53)

    def f_or(self, var, symb1, symb2):
        """
        Aplikuje konjunkci (logické A)/disjunkci (logické NEBO) na ⟨symb1⟩ a ⟨symb2⟩ nebo negaci na ⟨symb1⟩
        (NOT má pouze 2 operandy) a výsledek zapíše do ⟨var⟩ (všechny operandy jsou typu bool).
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 == 'bool' and v_type2 == 'bool':

            if value1 == 'true' or value2 == 'true':
                self.Frame[frame][name] = "int@true"
            else:
                self.Frame[frame][name] = "int@false"

        else:
            exit_error("Incompatible parameters", 53)

    def f_not(self, var, symb):
        """
        Aplikuje konjunkci (logické A)/disjunkci (logické NEBO) na ⟨symb1⟩ a ⟨symb2⟩ nebo negaci na ⟨symb1⟩
        (NOT má pouze 2 operandy) a výsledek zapíše do ⟨var⟩ (všechny operandy jsou typu bool).
        :param var:
        :param symb:
        """
        frame, name = self.var_analyzer(var, True)

        v_type, value = self.symb_var_split(symb)

        if v_type == 'bool':

            if value.lower() == 'true':
                self.Frame[frame][name] = "int@false"
            else:
                self.Frame[frame][name] = "int@true"

        else:
            exit_error("Incompatible parameters", 53)

    def int_to_char(self, var, symb):
        """
        Číselná hodnota ⟨symb⟩ je dle Unicode převedena na znak, který tvoří jednoznakový
        řetězec přiřazený do ⟨var⟩. Není-li ⟨symb⟩ validní ordinální hodnota znaku
        v Unicode (viz funkce chr v Python 3), dojde k chybě 58.
        :param var:
        :param symb:
        """
        frame, name = self.var_analyzer(var, True)

        v_type, value = self.symb_var_split(symb)

        if v_type != 'int':
            exit_error("Wrong imput for int to char", 53)

        try:
            result = chr(int(value))
        except ValueError:
            exit_error("Wrong range of int to char", 58)
            return

        self.Frame[frame][name] = "string@" + result

    def stri_to_int(self, var, symb1, symb2):
        """
        Do ⟨var⟩ uloží ordinální hodnotu znaku (dle Unicode) v řetězci ⟨symb1⟩ na pozici ⟨symb2⟩
        (indexováno od nuly).
        Indexace mimo daný řetězec vede na chybu 58. Viz funkce ord v Python 3.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != 'string' and v_type2 != 'int':
            exit_error("Wrong input of stri_to_int", 53)

        if int(value2) >= len(value1):
            exit_error("Index out of range, stri_to_int", 58)

        self.Frame[frame][name] = "int@" + str(ord(value1[int(value2)]))

    def read(self, var, v_type):
        """
        Načte jednu hodnotu dle zadaného typu ⟨type⟩ ∈ {int, string, bool} a uloží tuto hodnotu
        do proměnné ⟨var⟩. Načtení proveďte vestavěnou funkcí input() jazyka Python 3,
        pak proveďte konverzi na specifikovaný typ ⟨type⟩. Při převodu vstupu na typ bool
        nezáleží na velikosti pís- men a řetězec „true􏰀 se převádí na bool@true,
        vše ostatní na bool@false. V případě chybného vstupu bude do proměnné ⟨var⟩ uložena implicitní hodnota
        (dle typu 0, prázdný řetězec nebo false).
        :param var:
        :param v_type:
        """
        if v_type['Text'].lower() != 'int' and v_type['Text'].lower() != 'string' and v_type['Text'].lower() != 'bool':
            exit_error("Wrong parameters for read", 53)

        frame, name = self.var_analyzer(var, True)

        value = input()
        negative = False

        if v_type['Text'].lower() == 'int':
            if value[0] == '-':
                value = value[1:]
                negative = True

            if value.isdigit():
                if negative:
                    self.Frame[frame][name] = "int@" + "-" + str(value)
                else:
                    self.Frame[frame][name] = "int@" + str(value)
            else:
                self.Frame[frame][name] = "int@0"

        elif v_type['Text'].lower() == 'string':
            if isinstance(value, str):
                self.Frame[frame][name] = "string@" + value
            else:
                self.Frame[frame][name] = "string@"

        elif v_type['Text'].lower() == 'bool':
            if value.lower() == 'true':
                self.Frame[frame][name] = "bool@true"
            else:
                self.Frame[frame][name] = "bool@false"
        else:
            exit_error("Internal input error", 58)  # impossible

    def write(self, symb):
        """
        Vypíše hodnotu ⟨symb⟩ na standardní výstup. Až na typ bool je formát výpisu kompatibilní s příkazem
        print jazyka Python 3.
        :param symb:
        """

        def replace(match):
            return chr(int(match.group(1)))

        v_type, value = self.symb_var_split(symb)

        regex = re.compile(r"\\(\d{1,3})")
        value = regex.sub(replace, value)

        print(value)

    def concat(self, var, symb1, symb2):
        """
        Do proměnné ⟨var⟩ uloží řetězec vzniklý konkatenací dvou řetězcových operandů ⟨symb1⟩ a ⟨symb2⟩
        (jiné typy nejsou povoleny).
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != "string" and v_type2 != "string":
            exit_error("Wrong input type for concat", 53)

        result = value1 + value2

        self.Frame[frame][name] = "string@" + result

    def strlen(self, var, symb):
        """
        Zjistí počet znaků (délku) řetězce v ⟨symb⟩ a tato délka je uložena jako celé číslo do ⟨var⟩.
        :param var:
        :param symb:
        """
        frame, name = self.var_analyzer(var, True)

        v_type, value = self.symb_var_split(symb)

        if v_type != 'string':
            exit_error("Wrong input for strlen", 53)

        self.Frame[frame][name] = "int@" + str(len(value))

    def getchar(self, var, symb1, symb2):
        """
        Do ⟨var⟩ uloží řetězec z jednoho znaku v řetězci ⟨symb1⟩ na pozici ⟨symb2⟩
        (indexováno celým číslem od nuly). Indexace mimo daný řetězec vede na chybu 58.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != 'string' or v_type2 != 'int':
            exit_error("Wrong input types for getchar", 53)

        if int(value2) >= len(value1):
            exit_error("Out of range index in getchar", 58)

        self.Frame[frame][name] = "string@" + value1[int(value2)]

    def setchar(self, var, symb1, symb2):
        """
        Zmodifikuje znak řetězce uloženého v proměnné ⟨var⟩ na pozici ⟨symb1⟩ (indexováno celočíselně od nuly)
        na znak v řetězci ⟨symb2⟩ (první znak, pokud obsahuje ⟨symb2⟩ více znaků). Výsledný řetězec je opět
        uložen do ⟨var⟩. Při indexaci mimo řetězec ⟨var⟩ nebo v případě prázdného řetězce v ⟨symb2⟩
        dojde k chybě 58.
        :param var:
        :param symb1:
        :param symb2:
        """
        frame, name = self.var_analyzer(var, True)

        v_type, value = self.symb_var_split(var)
        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type != 'string' or v_type1 != 'int' or v_type2 != 'string':
            exit_error("Wrong input arguments for setchar", 53)

        if value2 == '':
            exit_error("Wrong input char in setchar", 58)

        if int(value1) >= len(value):
            exit_error("Index out of range in setchar", 58)

        if len(value2) < 1:
            exit_error("Invalid symbol to set", 58)

        value[int(value1)] = value2[0]

        self.Frame[frame][name] = "string@" + value

    def type(self, var, symb):
        """
        Dynamicky zjistí typ symbolu ⟨symb⟩ a do ⟨var⟩ zapíše řetězec značící tento typ (int, bool nebo string).
        Je-li ⟨symb⟩ neinicializovaná proměnná, označí její typ prázdným řetězcem.
        :param var:
        :param symb:
        """
        frame, name = self.var_analyzer(var, True)

        v_type, value = self.symb_var_split(symb)

        if v_type.lower() is None:
            self.Frame[frame][name] = "string@"
        elif v_type.lower() == 'int':
            self.Frame[frame][name] = "string@int"
        elif v_type.lower() == 'bool':
            self.Frame[frame][name] = "string@bool"
        elif v_type.lower() == 'string':
            self.Frame[frame][name] = "string@string"
        else:
            exit_error("Invalid type", 53)

    def label(self, label):
        """
        Speciální instrukce označující pomocí návěští ⟨label⟩ důležitou pozici v kódu jako potenciální
        cíl libovolné skokové instrukce. Pokus o redefinici existujícího návěští je chybou 52.
        :param label:
        """
        # label_analysis is doing this work, this is only decoration :-)

    def jump(self, label):
        """
        Provede nepodmíněný skok na zadané návěští ⟨label⟩.
        :param label:
        :return: Line of Code
        """
        if label['Text'] in self.labels:
            return int(self.labels[label['Text']]) - 1  # -1 because we have to deal with i += 1
        else:
            exit_error("Undefined label name", 52)

    def jumpifeq(self, label, symb1, symb2, index):
        """
        Pokud jsou ⟨symb1⟩ a ⟨symb2⟩ stejného typu (jinak chyba 53) a zároveň se jejich hodnoty rovnají,
        tak provede skok na návěští ⟨label⟩.
        :param label:
        :param symb1:
        :param symb2:
        :param index:
        """
        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != v_type2:
            exit_error("Uncomperable symbols", 53)

        if value1 == value2:
            if label['Text'] in self.labels:
                return int(self.labels[label['Text']]) - 3  # -3 because we have to deal with i+=3
            else:
                exit_error("Undefined label name!", 52)
        else:
            return index

    def jumpifneq(self, label, symb1, symb2, index):
        """
        Jsou-li ⟨symb1⟩ a ⟨symb2⟩ stejného typu (jinak chyba 53), ale různé hodnoty,
        tak provede skok na návěští ⟨label⟩.
        :param label:
        :param symb1:
        :param symb2:
        :param index:
        """
        v_type1, value1 = self.symb_var_split(symb1)
        v_type2, value2 = self.symb_var_split(symb2)

        if v_type1 != v_type2:
            exit_error("Uncomperable symbols", 53)

        if value1 != value2:
            if label['Text'] in self.labels:
                return int(self.labels[label['Text']]) - 3  # -3 because we have to deal with i+=3
            else:
                exit_error("Undefined label name", 52)
        else:
            return index

    def dprint(self, symb):
        """
        Předpokládá se, že vypíše zadanou hodnotu ⟨symb⟩ na standardní chybový výstup (stderr).
        :param symb:
        """
        v_type, value = self.symb_var_split(symb)
        sys.stderr.write(v_type + ": " + value)

    def f_break(self):
        """
        Předpokládá se, že na standardní chybový výstup (stderr) vypíše stav interpretu
        (např. pozice v kódu, obsah rámců, počet vykonaných instrukcí) v danou chvíli
        (tj. během vykonávání této instrukce).
        """
        global i
        loc = "Number of xml elements: " + str(i) + "\n"
        sys.stderr.write(loc)
        sys.stderr.write(str(self.Frame['GF']) + "\n")
        if 'LF' in self.Frame:
            sys.stderr.write(str(self.Frame['LF']) + "\n")

        if 'TF' in self.Frame:
            sys.stderr.write(str(self.Frame['TF']) + "\n")


def exit_error(message, code):
    """
    Terminates the programme
    :param message: Message on stderr
    :param code: Exit code
    :return:
    """

    sys.stderr.write(message)
    in_file.close()
    sys.exit(code)


def script_help():
    """
    Prints help for command line using
    :return:
    """

    print("help")   # TODO
    sys.exit(0)


def parsing(xml_tree):
    """
    Transforming xml_tree to list
    :param xml_tree:
    :return: list of dictionaries with tag, attribute dictionary and text of instruction or argument
    """

    xml_list = []

    for element in xml_tree.iter():
        if re.match(r'\n +', str(element.text)):
            xml_list.append({'Tag': element.tag, 'Attrib': element.attrib, 'Text': ''})
        else:
            xml_list.append({'Tag': element.tag, 'Attrib': element.attrib, 'Text': element.text})

    return xml_list


# noinspection PyTypeChecker
def interpret(xml_tree):
    """
    Interpretation of xml input
    :param xml_tree: ElementTree created from input
    :return:
    """

    xml_list = parsing(xml_tree)
    core = Core()

    core.label_analysis(xml_list)

    if xml_list[0]['Tag'] != 'program' or xml_list[0]['Attrib']['language'] != 'IPPcode18':  # validation of program
        exit_error("Wrong input code", 52)

    global i
    i = 1
    while i < len(xml_list):

        # noinspection PyTypeChecker
        if xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'MOVE':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2':
                core.move(xml_list[i+1], xml_list[i+2])
                i += 1
            else:
                exit_error("Wrong parameters of MOVE", 53)

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'CREATEFRAME':
            core.createframe()

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'PUSHFRAME':
            core.pushframe()

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'POPFRAME':
            core.popframe()

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'DEFVAR':
            if xml_list[i+1]['Tag'] == 'arg1':
                core.defvar(xml_list[i+1])
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'CALL':
            if xml_list[i+1]['Tag'] == 'arg1':
                i = core.call(xml_list[i+1], i)
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'RETURN':
            i = core.f_return()

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'PUSHS':
            if xml_list[i+1]['Tag'] == 'arg1':
                core.pushs(xml_list[i+1])
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'POPS':
            if xml_list[i+1]['Tag'] == 'arg1':
                core.pops(xml_list[i+1])
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'ADD':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.add(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'SUB':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.sub(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'MUL':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.mul(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'IDIV':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.idiv(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'LT':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.lt(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'GT':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.gt(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'EQ':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.eq(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'AND':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.f_and(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'OR':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.f_or(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'NOT':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2':
                core.f_not(xml_list[i+1], xml_list[i+2])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'INT2CHAR':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2':
                core.int_to_char(xml_list[i+1], xml_list[i+2])
                i += 2

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'STRI2INT':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.stri_to_int(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'READ':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2':
                core.read(xml_list[i+1], xml_list[i+2])
                i += 2

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'WRITE':
            if xml_list[i+1]['Tag'] == 'arg1':
                core.write(xml_list[i+1])
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'CONCAT':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.concat(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'STRLEN':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2':
                core.strlen(xml_list[i+1], xml_list[i+2])
                i += 2

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'GETCHAR':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.getchar(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'SETCHAR':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                core.setchar(xml_list[i+1], xml_list[i+2], xml_list[i+3])
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'TYPE':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2':
                core.type(xml_list[i+1], xml_list[i+2])
                i += 2

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'LABEL':
            if xml_list[i+1]['Tag'] == 'arg1':
                core.label(xml_list[i+1])
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'JUMP':
            if xml_list[i+1]['Tag'] == 'arg1':
                i = core.jump(xml_list[i+1])
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'JUMPIFEQ':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                i = core.jumpifeq(xml_list[i+1], xml_list[i+2], xml_list[i+3], i)
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'JUMPIFNEQ':
            if xml_list[i+1]['Tag'] == 'arg1' and xml_list[i+2]['Tag'] == 'arg2' and xml_list[i+3]['Tag'] == 'arg3':
                i = core.jumpifneq(xml_list[i+1], xml_list[i+2], xml_list[i+3], i)
                i += 3

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'DPRINT':
            if xml_list[i+1]['Tag'] == 'arg1':
                core.dprint(xml_list[i+1])
                i += 1

        elif xml_list[i]['Tag'] == 'instruction' and xml_list[i]['Attrib']['opcode'] == 'BREAK':
            core.f_break()

        i += 1  # while increment


def main():
    """
    Handle the arguments and running interpret itself
    :return:
    """

    global in_file
    in_file = sys.stdin

    # arguments
    if len(sys.argv) == 2:
        opts, args = getopt.getopt(sys.argv[1:], ["help", "source="], ["help", "source="])

        for arg, val in opts:
            if arg in ["-help", "--help"]:
                script_help()

            elif arg in ["-source", "--source"]:

                try:
                    in_file = open(val, "r")
                except IOError:
                    exit_error("Can't open the file", 99)

            else:
                exit_error("Wrong arguments, use --help", 10)

    elif len(sys.argv) != 1:
        exit_error("Wrong number of arguments, use --help", 10)

    xml_tree = ElementTree.parse(in_file)
    interpret(xml_tree)

    in_file.close()
    sys.exit(0)


if __name__ == "__main__":
    main()
