#!/usr/bin/env python3
#Tomas Lapsansky
#FIT VUT Brno
#ISJ 4. Projekt
"""Zadanie 4. projektu"""

def balanced_paren(parenstr):
	"""testuje spravne uzatvorkovanie"""
	symbol_array = []

	for symbol in parenstr:
		if symbol is '(':
			symbol_array.append(symbol)
		elif symbol is '[':
			symbol_array.append(symbol)
		elif symbol is '{':
			symbol_array.append(symbol)
		elif symbol is '<':
			symbol_array.append(symbol)
			
		elif symbol is ')':
		
			if symbol_array[-1] is '(':
				symbol_array = symbol_array[0:-1]
			else:
				return False
			
		elif symbol is ']':
			
			if symbol_array[-1] is '[':
				symbol_array = symbol_array[0:-1]
			else:
				return False
			
		elif symbol is '}':
			
			if symbol_array[-1] is '{':
				symbol_array = symbol_array[0:-1]
			else:
				return False
			
		elif symbol is '>':
			
			if symbol_array[-1] is '<':
				symbol_array = symbol_array[0:-1]
			else:
				return False
			
	return True
	
def caesar_list(word, *key):
	"""zasifruje word pomocou kluca algoritmom caesar list"""
	if len(key) == 0:
		key = [1, 2, 3]
	else:
		key = key[0]
	
	string = ''
	index = 0
	
	for symbol in word:
		
		if ord(symbol) > 122 or ord(symbol) < 96:
			raise ValueError
			
		symbol = ord(symbol) + key[index]
		if symbol > 122:
			symbol = symbol - 26
		symbol = chr(symbol)
		
		string = string + symbol
		
		index = index + 1
		if index==len(key):
			index = 0
		
	return string
	
def caesar_varnumkey(*arg):
	"""caesar list argumentacne inak"""
	if len(arg) == 0:
		raise ValueError
		
	if len(arg) == 1:
		return caesar_list(arg[0], [1, 2, 3])
	else:
		return caesar_list(arg[0], arg[1:])
