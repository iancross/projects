#returns a sorted list of sorted words in lowercase
#this allows us to do fast comparisons
def preprocess_list(wordlist):
	length = len(wordlist)
	for index in range(length):
		wordlist[index] = ''.join(sorted(wordlist[index].lower()))
	wordlist.sort()
	return wordlist

#returns the number of anagrams in a list of words
def anagram_count(wordlist):
	wordlist = preprocess_list(wordlist)
	total_count = 0
	i=0
	length = len(wordlist)
	while i < length:
		test_word = wordlist[i]
		count = 1
		ana_exists = False
		i+=1
		while i < length and test_word == wordlist[i]:
			i+=1
			count +=1
			ana_exists = True
		if ana_exists:
			total_count += count
	return total_count

f = open('/usr/share/dict/words')
wordlist = f.read().split('\n')
print anagram_count(wordlist)
