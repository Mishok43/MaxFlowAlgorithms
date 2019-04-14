from os import listdir
from os.path import isfile, join
for f in listdir("C:\\Users\\devmi\\source\\repos\\MaxFlowAlgorithms\\inputs"):
	s = f
	f = f[6:]
	i = f.find("_")
	print(f[:i] + " " +  s)