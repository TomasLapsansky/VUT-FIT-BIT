#!/usr/bin/env python3
#Tomas Lapsansky
#FIT VUT Brno
#ISJ 7. Projekt
"""ISJ 7. Projekt"""

class Log():
	"""Log class"""

	def __init__(self, filename):
		"""Log class init"""
		self.output=open(filename, "w")
		
	def __enter__(self):
		"""Log class enter"""
		self.output.write("Begin\n")
		return self
	
	def __exit__(self, exc_type, exc_val, exc_tb):
		"""Log class exit"""
		self.output.write("End\n")
		self.output.close()
		
	def logging(self, log):
		"""Log class logging"""
		self.output.write(log + "\n")
		

class TooManyCallsError(Exception):
	"""Error class"""
	
	pass

def limit_calls(max_calls = 2, error_message_tails = "called too often"):
	"""Limit calls decorator"""

	def decorate(func):
		"""Limit decorator"""
	
		def wrapper(*args,**kargs):
			"""Limit wrapper"""
			wrapper.calls += 1
			
			if wrapper.calls > max_calls:
			
				error_message = "function \"" + str(func.__name__) + "\" - " + error_message_tails
				raise TooManyCallsError(error_message)
				
			ret = func(*args,**kargs)
			return ret
			
		wrapper.calls = 0
		return wrapper
		
	return decorate
	
def ordered_merge(*args, **kwargs):
	"""Ordered merge"""
	
	ret = list()
	arg = list()
	
	if kwargs.__contains__("selector"):
		selector = kwargs["selector"]
	else:
		return ret
	
	for i in args:
		arg.append(list(i))
	
	for index in selector:
		ret.append(arg[index][0])
		arg[index].pop(0)
		
	return ret

if __name__ == "__main__":
	
	print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11), selector = [2,3,0,1,3,1])))
	
	import math
	
	@limit_calls(1, 'that is too much')
	def pyth(a,b):
		c = math.sqrt(a**2 + b ** 2)
		return c
		
	print(pyth(3,4))
	print(pyth(6,8))
	
	with Log('mylog.txt') as logfile:
		logfile.logging('Test1')
		logfile.logging('Test2')
		a = 1/0
		logfile.logging('Test3')
