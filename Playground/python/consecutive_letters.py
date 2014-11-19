'''Write a method that takes a string, in this format 
"aabbaadddc". Encode the string by counting the consecutive letters.
 Ex: "a2b2a2d3c1'''

word = "aabbaadddc"
consec = []


size = len(word)
count = 0
current_letter = word[0]
i = 0

while i < size:
	if word[i] == current_letter:
		count += 1

	else:
		consec.append(current_letter)
		current_letter = word[i]
		consec.append (str(count))
		count = 1
	i +=1
consec.append(current_letter)
consec.append(str(count))

print ''.join(consec)