#!/usr/bin/env python3
#Tomas Lapsansky
#FIT VUT Brno
#ISJ 6. Projekt
"""ISJ 6. Projekt"""

import itertools

def first_nonrepeating(string):
	"""Returns first non-repeating letter of string"""
	first_use = list()
	repetitive = list()
	
	for symbol in string:
	
		if symbol in first_use:
			repetitive.append(symbol)
		else:
			first_use.append(symbol)
	
	for symbol in string:
		if not symbol in repetitive:
			return symbol
			
	return None
	
def combine4(list_of_int, result_int):
	"""Return every possible combinations of numbers in list_of_int which result is result_int"""
	strings = list()
	
	list_of_symbols = ["+","-","*","/"]
	
	permutation_symbol = list()
	permutation_int = list()
	
	for subset in itertools.product(list_of_symbols, repeat = 3):
		permutation_symbol.append(subset)
	
	for subset in itertools.permutations(list_of_int, 4):
		permutation_int.append(subset)
	
	#for small_list in permutation_symbol:
	#print(permutation_symbol)
	#print(permutation_int)
	
	for int_list in permutation_int:
		
		for symbol_list in permutation_symbol:
#			....
			string = str(int_list[0]) + symbol_list[0] + str(int_list[1]) + symbol_list[1] + str(int_list[2]) + symbol_list[2] + str(int_list[3])

			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
				
#			(..)..
			string = "(" + str(int_list[0]) + symbol_list[0] + str(int_list[1]) + ")" + symbol_list[1] + str(int_list[2]) + symbol_list[2] + str(int_list[3])
			
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
	
#			.(..).
			string = str(int_list[0]) + symbol_list[0] + "(" + str(int_list[1]) + symbol_list[1] + str(int_list[2]) + ")" + symbol_list[2] + str(int_list[3])
			
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
				
#			..(..)
			string = str(int_list[0]) + symbol_list[0] + str(int_list[1]) + symbol_list[1] + "(" + str(int_list[2]) + symbol_list[2] + str(int_list[3]) + ")"
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
			
#			(..)(..)
			string = "(" + str(int_list[0]) + symbol_list[0] + str(int_list[1]) + ")" + symbol_list[1] + "(" + str(int_list[2]) + symbol_list[2] + str(int_list[3]) + ")"
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
				
#			(...).
			string = "(" + str(int_list[0]) + symbol_list[0] + str(int_list[1]) + symbol_list[1] + str(int_list[2]) + ")" + symbol_list[2] + str(int_list[3])

			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
			
#			.(...)
			string = str(int_list[0]) + symbol_list[0] + "(" + str(int_list[1]) + symbol_list[1] + str(int_list[2]) + symbol_list[2] + str(int_list[3]) + ")"
			
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
				
#			((..).).
			string = "((" + str(int_list[0]) + symbol_list[0] + str(int_list[1]) + ")" + symbol_list[1] + str(int_list[2]) + ")" + symbol_list[2] + str(int_list[3])
			
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
				
#			(.(..)).
			string = "(" + str(int_list[0]) + symbol_list[0] + "(" + str(int_list[1]) + symbol_list[1] + str(int_list[2]) + "))" + symbol_list[2] + str(int_list[3])
			
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
				
#			.((..).)
			string = str(int_list[0]) + symbol_list[0] + "((" + str(int_list[1]) + symbol_list[1] + str(int_list[2]) + ")" + symbol_list[2] + str(int_list[3]) + ")"
			
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
				
#			.(.(..))
			string = str(int_list[0]) + symbol_list[0] + "(" + str(int_list[1]) + symbol_list[1] + "(" + str(int_list[2]) + symbol_list[2] + str(int_list[3]) + "))"
			
			try:
				if eval(string) == result_int:
					strings.append(string)
			except:
				True
	
	return(list(set(strings)))
	
if __name__ == "__main__":

	assert first_nonrepeating("lool") == None
	assert first_nonrepeating("tooth") == "h"
	assert first_nonrepeating("alaaaa1la") == "1"
	assert first_nonrepeating("testingstring") =="e"
	assert first_nonrepeating("something") == "s"
	assert first_nonrepeating("valve") == "a"
	assert first_nonrepeating("repeterepete") == None
	assert first_nonrepeating("hahaha") == None
	
	print(combine4([1, 2, 3, 4], 17))
	
