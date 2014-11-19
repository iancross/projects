"""Given a positive int "N". and an array of numbers ranging from 0-9 (say array name is arr). 
print all numbers from 0 to N which include any number from "arr". 

example: 
i/p: N=20 
arr={2,3} 

o/p: 2,3,12,13,20"""

N = 23
tests = [2,3]
output = []
for i in range(0,N+1):
	for test_num in tests:
		if str(test_num) in str(i):
			output.append (i)
			break
print output 