from os import system

for i in range(0,9):
	temp = "java weka.classifiers.lazy.IBk" + " -t" + " ~/Dropbox/compsci/Comp135/a2/diabetes" + str(i) + "training.arff" + " -T" + " ~/Dropbox/compsci/Comp135/a2/diabetes" + str(i) + "testing.arff" + " -i"
	system(temp)
	raw_input("Press Enter to continue...")


print "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
for i in range(0,9):
	temp = "java weka.classifiers.trees.J48" + " -t" + " ~/Dropbox/compsci/Comp135/a2/diabetes" + str(i) + "training.arff" + " -T" + " ~/Dropbox/compsci/Comp135/a2/diabetes" + str(i) + "testing.arff" + " -i"
	system(temp)
	raw_input("Press Enter to continue...")
