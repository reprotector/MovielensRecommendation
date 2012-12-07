import shlex
from math import pow, sqrt

def RMS(fileName):
  f = open(fileName)
	lines = f.readlines()
	f.close()
	
	n = 0
	rms = 0.0
	for line in lines:
		split = shlex.split(line)
		user =  int(split[0]) - 1
		movie = int(split[1]) - 1
		rate = int(split[2])
		n = n + 1
		rms = rms + pow(table[user][movie] - rate, 2)
	
	rms = sqrt(rms/n)
	return rms