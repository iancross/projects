#export CLASSPATH=/Users/Administrator/Dropbox/compsci/Comp135/weka-3-6-11/weka.jar:$CLASSPATH


#got help from Mathematica, stackoverflow dot product forum, and most importantly Mengfei
import numpy
import math
from random import shuffle
from random import randrange
from random import choice 
from numpy import asarray,array, dot, random

#takes the dotProduct of 2 lists
def dotProduct(weights, currList):
	return sum(a*b for a,b in zip(weights,currList))

#takes an int and this is where the algo makes the guess
def sign(x):
	if x < 0:
		return -1
	else:
		return 1

#takes the weights and the current list and updates them accordingly
def updateWeights(weights, currList,rate, correct):
	for i,val in enumerate(currList):
		weights[i] += rate*val*correct
	return weights

def calculateTau(trainingData):
	total = 0
	count = 0
	for line in trainingData:
		total += math.sqrt(dotProduct(line[0],line[0]))
		count += 1
	return 0.1* total/count

def PwM(trainingData,testingData,iterations, w):

		#shuffle these values so we don't get all positives at the beginning
	shuffle(trainingData)
	shuffle(testingData)
	#training
	tau = calculateTau(trainingData)

	learningRate = 0.1 
	for x in range(0, iterations):
		for i in trainingData:
			x, expected = i 
			O = sign(dotProduct(w,x))
			if (expected*dotProduct(w,x)) < tau:
				w = updateWeights(w,x,learningRate, expected);

	correct = 0.0
	wrong = 0.0
	for i in testingData:
		x,expected = i
		O = sign(dotProduct(w,x))
		if O != expected:
			wrong+=1
		else:
			correct+=1
	accuracy = (correct)/(wrong+correct)
	return w,accuracy

def getStats(data):
	averages = []
	for i in range(len(data[0])):
		for d in data:
			averages += [d[i]]
		avg = getAvg(averages)
		std = getSTD(avg,averages)
		print  avg,",",std
		averages = []

def getAvg(acc):
	total = 0.0
	for a in acc:
		total += a
	avg = total/float(len(acc))
	return avg

def getSTD(avg, acc):
	total = 0.0
	for a in acc:
		total += (avg-a) ** 2
	stdDeviation = total/float(len(acc))
	return stdDeviation


dataLines = []

infile = open("diabetes.arff","r")
data = False
for line in infile:
	if data:
		curr = line[:-1]
		curr = curr.split(',');

		#add a one at the end of the array for the threshold
		array1 = curr[0:-2] + [1]
		for i, val in enumerate(array1):
			array1[i] = float(val)
		
		curr = (array1, int(curr[-1]))
		dataLines.append(curr)

	if line == "@data\n":
		data = True

totalAccPwM = []
wPwM = [0] * len(dataLinesT[0][0])

#lists of the accuracies taken from each iteration

currAccPwM = []
for numIters in range(1,50):
	wPwM,accPwM = PwM(train,test,numIters,wPwM)
	currAccPwM += [accPwM]
totalAccPwM.append(currAccPwM)

print "PwM ----------------------------------------"
print "accuracies, standard deviation"
getStats(totalAccPwM)








