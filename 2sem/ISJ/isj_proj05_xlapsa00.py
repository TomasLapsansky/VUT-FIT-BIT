#!/usr/bin/env python3
#Tomas Lapsansky
#FIT VUT Brno
#ISJ 5. Projekt
"""Zadanie 5. projektu"""

class Polynomial:
	"""Definicia polynomial class"""
	def __init__(self, *args, **kwargs):
		"""Inicializacia polynomu na list"""
		
		self.args = args
		self.kwargs = kwargs
		self.constants = list()
		
		if not len(args) == 0 and isinstance(args[0], list):
		
			self.constants = args[0]
			
		elif not self.kwargs:
		
			for arg in self.args:
				self.constants.append(arg)
				
		else:
			constants = {index.replace('x', ''): kwargs[index] for index in kwargs.keys()}
			constants = {int(key):int(value) for key, value in constants.items()}
			
			maxkey = max(constants.keys())
			
			for i in range(0, maxkey+1):
				if not i in constants.keys():
					constants[i] = 0
			
			for i in range(0, maxkey+1):
				self.constants.append(constants[i])
				
			while self.constants[-1] == 0 and not len(self.constants) == 1:
				self.constants.pop()
			
			
	def __str__(self):
		"""Vypis Polynomial do korektneho tvaru"""
		
		zero=1
		for i in self.constants:
			if not i == 0:
				zero=0
				
		if zero == 1:
			return "0"
			
		printstr = ""
		for index, value in reversed(list(enumerate(self.constants))):
			if index == 1:
			
				if printstr:
					if value == 1:
						printstr = printstr + " + x"
					elif value == -1:
						printstr = printstr + " - x"
					elif value > 0:
						printstr = printstr + " + " + str(value) + "x"
					elif value < 0:
						printstr = printstr + " - " + str(-value) + "x"
					elif value == 0:
						pass
				else:
					if value == 1:
						printstr = "x"
					elif value == -1:
						printstr = "- x"
					elif value > 0:
						printstr = str(value) + "x"
					elif value < 0:
						printstr = "- " + str(-value) + "x"
					elif value == 0:
						pass
						
			elif index == 0:
			
				if printstr:
					if value > 0:
						printstr = printstr + " + " + str(value)
					elif value < 0:
						printstr = printstr + " - " + str(-value)
					elif value == 0:
						pass
				else:
					if value > 0:
						printstr = str(value)
					elif value < 0:
						printstr = "- " + str(-value)
					elif value == 0:
						pass
						
			elif printstr:
				if value == 1:
					printstr = printstr + " + x^" + str(index)
				elif value == -1:
					printstr = printstr + " - x^" + str(index)
				elif value > 0:
					printstr = printstr + " + " + str(value) + "x^" + str(index)
				elif value < 0:
					printstr = printstr + " - " + str(-value) + "x^" + str(index)
				elif value == 0:
					pass
			else:
				if value == 1:
					printstr = "x^" + str(index)
				elif value == -1:
					printstr = "- x^" + str(index)
				elif value > 0:
					printstr = str(value) + "x^" + str(index)
				elif value < 0:
					printstr = "- " + str(-value) + "x^" + str(index)
				elif value == 0:
					pass
					
		return printstr
		
		
	def __eq__(self, self2):
		"""operator =="""
		return self.constants == self2.constants
		
	def __add__(self, self2):
		"""Sucet 2 Polynomial"""
	
		if not len(self.constants) == len(self2.constants):
			if len(self.constants) > len(self2.constants):
			
				for i in range(len(self2.constants), len(self.constants)):
					self2.constants.append(int(0))
					
			else:
				
				for i in range(len(self.constants), len(self2.constants)):
					self.constants.append(int(0))
				
			
		return Polynomial(list(x + y for x, y in zip(self.constants, self2.constants)))
		
		#alternative
		#temp = list()
		#
		#for i in range(0, len(self.constants)):
		#	temp.append(self.constants[i] + self2.constants[i])
		#
		#return Polynomial(temp)
		
	def derivative(self):
		"""Derivacia rovnice n-teho stupna"""
	
		constants = list()
		
		for i in range(1, len(self.constants)):
			constants.append(i * self.constants[i])
		
		return Polynomial(constants)
		
	def at_value(self, *args):
		"""Vypocet Y, pri viacerych argumentoch, robi ich rozdiel"""
		
		if len(args) == 1:
		
			result = 0.0
			my_x = float(args[0])
			
			for i in range(0, len(self.constants)):
				result = result + self.constants[i]*(my_x ** i)
		else:
			
			result1 = 0.0
			result2 = 0.0
			my_x1 = float(args[0])
			my_x2 = float(args[1])
			
			for i in range(0, len(self.constants)):
				result1 = result1 + self.constants[i]*(my_x1 ** i)
			for i in range(0, len(self.constants)):
				result2 = result2 + self.constants[i]*(my_x2 ** i)
			
			result = result2 - result1
		
		return result
		
	def multiply(a, b):
		"""Pomocne nasobenie potrebne pre __pow__"""
		c = [0]*(len(a) + len(b)-1)
		
		for i in range(len(a)):
			ai = a[i]
			for j in range(len(b)):
				c[i + j] += ai * b[j]

		return c
    
	def __pow__(self, n):
		"""Mocnina z N+"""
		
		c = [1]
		for i in range(n):
			c = Polynomial.multiply(c, self.constants)
		
		return Polynomial(c)


def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()
    pass
