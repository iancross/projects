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

def P (trainingData, testingData, iterations, w):
	#shuffle these values so we don't get all positives at the beginning
	shuffle(trainingData)
	shuffle(testingData)
	#training

	learningRate = 0.1 
	for x in range(0, iterations):
		for i in trainingData:
			x, expected = i 
			O = sign(dotProduct(w,x))
			if O != expected:
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


def AP(trainingData, testingData,iterations, v, w):
	#shuffle these values so we don't get all positives at the beginning
	shuffle(trainingData)
	shuffle(testingData)
	#training
	count = 0;
	learningRate = 0.1 
	for x in range(0, iterations):
		for i in trainingData:
			x, expected = i 
			O = sign(dotProduct(w,x))
			if O != expected:
				w = updateWeights(w,x,learningRate, expected);
				v = [x + y for x, y in zip(v, w)]
				count +=1

	correct = 0.0
	wrong = 0.0
	for i in testingData:
		x,expected = i
		O = sign(dotProduct(v,x))
		if O != expected:
			wrong+=1
		else:
			correct+=1
	accuracy = (correct)/(wrong+correct)
	return v,w,accuracy

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

def writeFile(train, currFile, test):
	outputFile = open("diabetes" + str(currFile) + test + ".arff", "w+")
	for line in initialLines:
		outputFile.write(line)
	for line in train:
		x = str(line[0]).strip('[]') + ',' + str(int(line[1])) + '\n'
		outputFile.write(x)

def getNextFold(data,dist):
	#eg.   getNextFold([1,2,3,4,5]) => [3,4,5,1,2]
	newData = [0] * len(data)

	for i,val in enumerate(data):
		newData[(i + dist)%len(data)] = val
	return newData

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


dataLinesT = []
dataLinesF = []
initialLines = []	
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
		if curr[1] == -1:
			dataLinesF.append(curr)
		else:
			dataLinesT.append(curr)
	else:
		initialLines.append(line)

	if line == "@data\n":
		data = True

numT = len(dataLinesT)
numF = len(dataLinesF)
total = numT + numF
numfolds = 10
foldSize = total /10
ratio = float(foldSize)/float(total)
numValuesT = int(numT*ratio)
numValuesF = int(numF*ratio)
v = [0] * len(dataLinesT[0][0])
#this loop does an iteration per fold and tests and trains

totalAccP = []
totalAccAP = []
totalAccPwM = []

for fold in range(0,numfolds):
	dataLinesT = getNextFold(dataLinesT,numValuesT)
	dataLinesF = getNextFold(dataLinesF,numValuesF)
	index = 0;

	test = dataLinesT[0:numValuesT] + dataLinesF[0:numValuesF]
	train = dataLinesT[numValuesT:-1] + dataLinesF[numValuesF:-1]
	
	wP = [0] * len(dataLinesT[0][0])
	wAP = [0] * len(dataLinesT[0][0])
	wPwM = [0] * len(dataLinesT[0][0])
	v = [0] * len(dataLinesT[0][0])

	v, wAP, accAP = AP(train,test,1,v, wAP)
	wP,acc = P(train,test,1,wP)
	wPwM,acc = PwM(train,test,1,wPwM)

	#lists of the accuracies taken from each iteration
	currAccP = []
	currAccAP = []
	currAccPwM = []
	for numIters in range(0,6):
		if numIters == 0:
				v, wAP, accAP = AP(train,test,1,v, wAP)
				wP,acc = P(train,test,1,wP)
				wPwM,accPwM = PwM(train,test,1,wPwM)
		else:
			v, wAP, accAP = AP(train,test,numIters*5,v, wAP)
			wP,acc = P(train,test,numIters*5,wP)
			wPwM,accPwM = PwM(train,test,numIters*5,wPwM)
			
		currAccAP +=[accAP]
		currAccP += [acc]
		currAccPwM += [accPwM]
	totalAccP.append(currAccP)
	totalAccAP.append(currAccAP)
	totalAccPwM.append(currAccPwM)
	writeFile(train,fold,"training")
	writeFile(test, fold, "testing")

print "P -----------------------------------------"
print "accuracies, standard deviation"
getStats(totalAccP)
print "AP -----------------------------------------"
print "accuracies, standard deviation"
getStats(totalAccAP)
print "PwM ----------------------------------------"
print "accuracies, standard deviation"
getStats(totalAccPwM)








