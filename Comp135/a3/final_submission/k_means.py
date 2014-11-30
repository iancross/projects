#cluster representation:
#cluster means = [[1,2,3],[4,1,2],[4,3,2]]
#point = [x1,x2,x3 ..., class label, cluster index]

import random
import math

#update the cluster label for each index
def update_clusters(means, points):
	for i,(point,c,cluster_index) in enumerate(points):
		min_dist = -1.0
		for j,(mean,num_points) in enumerate(means):
			curr_dist = dist(mean,point)
			if curr_dist < min_dist or min_dist < 0: 
				min_dist = curr_dist
				cluster_index = j
		points[i] = (point,c,cluster_index)
	num_attrib = len(point)
	means = update_means(means,num_attrib,points)
	return means,points

def update_means(means,attributes,points):
	#init means to all zeros
	for i,(mean, num_points) in enumerate(means):
		means[i] = ([0]*attributes,0)
	for (point,c,cluster_index) in points:
		for i,val in enumerate(point):
			(a,b) = means[cluster_index]
			a[i]+=val
			means[cluster_index] = (a,b)
		means[cluster_index] = (a,b+1)
	for i,(mean,num_points) in enumerate(means):
		if num_points != 0:
			means[i] = ([x / num_points for x in mean],num_points)
	return means


#returns an array of points that we initially use as the means
def get_k_random_points(data, k,length):
	points = []
	count = 0
	while count < k and k<=length:
		point = data[random.randrange(0,length,1)][0]
		if point not in points:
			points.append((point,0))
			count +=1
	return points
def dist(point1,point2):
	total_dist = 0
	for i, val in enumerate(point1):
		total_dist += abs(val - point2[i])
	return total_dist
def parse_data():
	dataLines = []
	initialLines = []	
	infile = open("Data/ionosphere.arff","r")
	print "ionosphere.arff"
	data = False
	for line in infile:
		if data:
			curr = line[:-1]
			curr = curr.split(',');

			array1 = curr[0:-1]
			for i, val in enumerate(array1):
				array1[i] = float(val)
			
			curr = (array1, (curr[-1]),0)
			dataLines.append(curr)
		else:
			initialLines.append(line)

		if line == "@data\n":
			data = True
	return dataLines
def calc_cluster_scatter(means,points):
	total_cs = 0
	for (point,class_label,cluster_index) in points:
		(mean, num_points) = means[cluster_index]
		total_cs += pow(dist(point,mean),2)
	return total_cs


#for each cluster, add up the classes associated with that cluster
def get_sums_a(table):
	sums_a = []
	for cluster_info in table:
		a_total = 0
		for key,value in cluster_info.items():
			a_total += value
		sums_a += [a_total]
	return sums_a

def get_sums_b(table,classes):
	sums_b = []
	for val in classes:
		total = 0	
		for cluster_info in table:
			total += cluster_info[val]
		sums_b += [total]
	return sums_b

def H(sums):
	N = float(sum(sums))
	total = 0
	for val in sums:
		total += (val/N)* math.log(val/N)
	return total*-1.0
def I(table, sums_a, sums_b,classes):
	N = float(sum(sums_b))
	total = 0.0
	for j,class_label in enumerate(classes): 	#for each class label
		for i,d in enumerate(table):			#for each cluster
			nij = d[class_label]
			ai = sums_a[i] 
			bj = sums_b[j]
			"""print "nij : ", nij
			print "ai : ", ai
			print "bj : ", bj
			print "\n\n"
			"""
			if ai !=0 and bj != 0 and nij !=0:
				total += (nij/N) * math.log((nij*N)/(ai*bj))
	return total
def init_class_structure(nmi_table):
	classes = []
	for table in nmi_table:
		for key,value in table.items():
			if key not in classes:
				classes += [key]
	for class_label in classes:
		for table in nmi_table:
			if class_label not in table:
				table[class_label] = 0
	return classes,nmi_table

def calc_NMI(means,points,num_classes):
	nmi_table = []
	classes = []
	for i,val in enumerate(means):
		nmi_table.append({})
	for (point,class_label,cluster_index) in points:
		if class_label in nmi_table[cluster_index]:
			nmi_table[cluster_index][class_label] += 1
		else:
			nmi_table[cluster_index][class_label] = 1
	
	classes,nmi_table = init_class_structure(nmi_table)
	sums_a = get_sums_a(nmi_table)
	sums_b = get_sums_b(nmi_table,classes)
	return (2*I(nmi_table,sums_a,sums_b,classes)) / (H(sums_a)+H(sums_b))


def sensitivity_to_init(points,num_classes):
	k = num_classes
	print "TotalCS, NMI"
	for i in range(0,10):	
		total_cs = 0.0
		temp_points = points
		means = get_k_random_points(temp_points, k, len(temp_points))
		iterations = 0
		while iterations < 10:
			means, temp_points = update_clusters(means,temp_points)
			iterations +=1
		total_cs = calc_cluster_scatter(means,temp_points)
		NMI = calc_NMI(means,temp_points, k)
		print total_cs, ",",NMI

def selecting_K(points):
	for k in range(1,16):
		#print "K is ", k
		total_cs = 0.0
		temp_points = points
		means = get_k_random_points(temp_points, k, len(temp_points))
		iterations = 0
		while iterations < 10:
			means, temp_points = update_clusters(means,temp_points)
			#print means, "\n\n\n", "--------------------------------"
			iterations +=1
			total_cs += calc_cluster_scatter(means,temp_points)
		print total_cs/10.0

#main
random.seed(0)
points = parse_data()
sensitivity_to_init(points,2) #change num_classes as needed.
selecting_K(points)
