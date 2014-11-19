word = "Today is the day"
word = list(word)
i = 0
size = len(word)
while i < size:
	testing = word[i].lower()
	temp = i + 1
	while temp < size:
		if testing == word[temp].lower():
			word.pop(temp)
			size -= 1
		else:
			temp += 1
	i+=1
print ''.join(word)