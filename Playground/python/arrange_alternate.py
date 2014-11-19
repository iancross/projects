'''Arrange the numbers in an array in alternating order. 
For example if the array is [a1, a2, a3, a4.. ]arrange the array such that b1<=b2>=b3<=b4 and so on. 
Sampe Input: 3 5 7 8 4 9 
Sample Output: 3 < 5 > 4 < 8 >7 < 9'''

numbers = [12,3,19,5,8,2]

numbers.sort()
size = len(numbers)
i=1
while i+2 < size:
	word1 = numbers[i]
	word2 = numbers[i+1]
	numbers[i] = word2
	numbers[i+1] = word1
	i+=2
print numbers