# java weka.classifiers.trees.J48 -t ~/Dropbox/compsci/Comp135/EEGTrainingData/EEGTrainingData.arff 
#-T ~/Dropbox/compsci/Comp135/EEGTrainingData.arff 
"""
from os import system

dataSetFeatures = ["", "_24","_34","_44","_54","_64","_74","_84","_94"]
classifiers = []
for i in dataSetFeatures:
	temp = "java weka.classifiers.lazy.IBk" + " -t" + " ~/Dropbox/compsci/Comp135/EEGTrainingData" + i + ".arff" + " -T" + " ~/Dropbox/compsci/Comp135/EEGTestingData" + i + ".arff" + " -i"
	system(temp)
	raw_input("Press Enter to continue...")
"""
from os import system

classifiers = []
for i in range(0,9):
	temp = "java weka.classifiers.trees.J48" + " -t" + " ~/Dropbox/compsci/Comp135/EEGTrainingData-54subset" + str(i) + ".arff" + " -T" + " ~/Dropbox/compsci/Comp135/EEGTestingData_54.arff" + " -i"
	system(temp)
	raw_input("Press Enter to continue...")