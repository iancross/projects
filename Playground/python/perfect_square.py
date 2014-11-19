#naive solution
'''def is_square (number):
	i = 0
	while i < number:
		if square (i) == number:
			return True
		i += 1
	return False

def square (num):
	count = num
	total = 0
	while count > 0:
		total += num
		count -= 1
	return total
'''

def is_square (number):
	sum1 = 1
	odd_num = 1
	i = 0
	while i < number:
		if sum1 == number:
			return True
		else:
			odd_num += 2
			sum1 += odd_num
		i+=1
	return False

while (1):
	number = int(raw_input())
	print is_square (number)
