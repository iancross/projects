#returns 1 if the words are anagrams of each other. 0 otherwise
def is_anagram (word1, word2):
	exists = 0
	if len(word1) != len(word2):
		return 0
	for i in word1:
		for j in word2:
			if i == j:
				exists = 1
		if not exists: 
			return 0
		else:
			exists = 0			
	return 1

'''def is_anagram(word1, word2):
	if word1 == word2:
		return 1
	else:
		return 0'''

def sort_words_in_list(wordlist):
	print wordlist
	length = len(wordlist)
	for index in range(length):
		wordlist[index] = wordlist[index].lower()
	print wordlist

def anagram_count(wordlist):
	sort_words_in_list(wordlist)
	index_array = []
	anagram_count = 0
	while wordlist:
		temp_count = 1		#initialize variables at start of loop
		ana_exists = False
		test_word = wordlist.pop(0)
		for index, word in enumerate(wordlist): 
			if is_anagram(test_word, word):
				temp_count += 1
				ana_exists = True
				index_array.append(index)
		if ana_exists:
			for i in reversed(index_array): #clean-up; delete words that
				wordlist.pop(i)				#have already been found
				index_array.remove(i)
			anagram_count += temp_count
			print anagram_count
	return anagram_count


#f = open('/Users/Administrator/Dropbox/compsci/ic_anagrams')
f = open('/usr/share/dict/words')
wordlist = f.read().split('\n')
print anagram_count(wordlist)

