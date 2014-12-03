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

def polyKernel(weights, currList, d):
	return (dotProduct(weights,currList) + 1) ** d

def RBFKernel(weights, currList, s):
	numer = math.pow(norm(weights)-norm(currList),2)
	denom = 2*math.pow(s,2)
	return pow(math.e, -(numer/denom))

def norm(vector):
	return math.sqrt(sum(a**2 for a in vector))
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

def calculateTauPrimal(trainingData):
	total = 0
	for line,_ in trainingData:
		total += norm(line)
	return 0.1* total/len(trainingData)

def calculateTauPoly(trainingData,d):
	total = 0
	for line,_ in trainingData:
		total += math.sqrt(polyKernel(line,line,d))
	return 0.1* total/len(trainingData)
def calculateTauRBF(trainingData,s):
	total = 0
	for line,_ in trainingData:
		total += math.sqrt(RBFKernel(line,line,d))
	return 0.1* total/len(trainingData)

def getTestValue(trainingData,x,alpha):
	total = 0
	for i,(vector,y) in enumerate(trainingData):
		total += alpha[i]*y*dotProduct(vector*x)
	return total

def PwMPrimal(trainingData,testingData,iterations, w, tau):
	#training
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

def PwMDual(trainingData,testingData,iterations,alpha,tau):
	#training
	learningRate = 0.1 
	for x in range(0, iterations):
		for i in trainingData:
			x, expected = i 
			value = getTestValue(trainingData,x,alpha) #where im at
			O = sign(value)
			if (expected*value < tau):
				w = updateWeights(w,x,learningRate, expected); #<- update alpha

	#testing
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

"""contains the training set in index 0 and the test set in 1"""
dataFiles = []

dataLines = []
for filenum in range(0,2):
	if filenum == 0:
		infile = open("backTrain.arff", "r")
	if filenum == 1:
		infile = open("backTest.arff", "r")
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

		if line[:5] == "@data":
			data = True
	"""print dataLines"""
	dataFiles += [dataLines]

train = dataFiles[0]
test = dataFiles[1]

totalAccPwM = []
wPwM = [0] * len(train[0][0])

#lists of the accuracies taken from each iteration
numIters = 50
tau = calculateTauPrimal(train)
_,accPwM = PwMPrimal(train,test,numIters,wPwM, tau)
totalAccPwM += [accPwM]

currAccPwM = []
for d in range(1,6):
	tau = calculateTauPoly(train, d)
	_,accPwM = PwMPrimal(train,test,numIters,wPwM, tau)
	totalAccPwM += [accPwM]

print totalAccPwM
for s in range(1,4):
	if s == 1:
		print "test"
		tau = calculateTauRBF(train, .1)
		_,accPwM = PwMPrimal(train,test,numIters,wPwM, tau)
	elif s == 2:
		tau = calculateTauRBF(train, .5)
		_,accPwM = PwMPrimal(train,test,numIters,wPwM, tau)
	elif s == 3:
		tau = calculateTauRBF(train, 1)
		_,accPwM = PwMPrimal(train,test,numIters,wPwM, tau)
	totalAccPwM += [accPwM]

print totalAccPwM







