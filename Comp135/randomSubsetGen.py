from random import shuffle
import random

infile = open("EEGTrainingData_54.arff","r")
data = False
initialLines = []
dataLines = []
for line in infile:
	if not data:
		initialLines.append(line)
	else:
		dataLines.append(line)
	if line == "@DATA\n":
		data = True
		raw_input("Press enter to continue...")


#write
interval = len(dataLines)/10-1
for i in range(0,10):
	outputFile = open("EEGTrainingData-54subset" + str(i) + ".arff", "w+")
	for line in initialLines:
		outputFile.write(line)
	print(dataLines[0])
	shuffle(dataLines)

	#randomNum = random.randrange(0,len(dataLines))
	for x in range(0,interval*(i+1)):
		outputFile.write(dataLines[x])
