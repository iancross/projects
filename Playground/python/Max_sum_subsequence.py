numbers = [-2,13,5,2,10]

max_so_far = numbers[0]
max_here = max_so_far
begin = 0
begin_temp = 0
end = 0

size = len(numbers)
for i in range(0,size):
	if max_here < 0:
		max_here = numbers[i]
		begin_temp = i
	else: 
		max_here += numbers[i]

	if max_here >= max_so_far:
		max_so_far = max_here
		begin = begin_temp
		end = i
print max_here

#prints the subsequence
for x in range(begin,end+1):
	print numbers[x],

