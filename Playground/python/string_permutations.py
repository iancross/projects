#returns a list of all possible anagrams/permutations of a word
def find_perm (word):
	final_list = []
	
	if len(word) == 1:        	#base case. permutation identity rule
		final_list.append(word)
	else:	
		for pos in range (len(word)):
			permutations = find_perm(word[0:pos]+word[pos+1:len(word)])
			for item in permutations:
				final_list.append(word[pos]+item)
	return final_list
find_perm ('ian')





