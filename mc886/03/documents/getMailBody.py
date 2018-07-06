import os
from nltk.corpus import stopwords
from nltk.stem.porter import PorterStemmer

i = 0
stopWords = set(stopwords.words('english'))
stopWords.add('\n')
stopWords.add("I'm")
stem = PorterStemmer()

for filename in os.listdir("docs/"):
        p = False
        with open("docs/" + filename, 'r') as file_read:
            with open("body/body" + str(i)+".txt", 'w') as file_write:
                i += 1
                for line in file_read:
                    if p:
                        line = line[:-1].split(' ')
                        newLine = ""
                        for word in line:
                            if word not in stopWords:
                                newLine += stem.stem(word) + ' '
                        file_write.write(newLine)
                    elif line[0] == "\n":
                        p = True
