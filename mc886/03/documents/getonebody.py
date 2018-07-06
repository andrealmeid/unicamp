from nltk.corpus import stopwords
from nltk.stem.porter import PorterStemmer


i = 0
stopWords = set(stopwords.words('english'))
stopWords.add('\n')
stopWords.add("I'm")
p = False
stem = PorterStemmer()

with open("docs/bdd813b54d849bd6f28467d96cf53e245ab6d657", 'r') as file_read:
    with open("body/bo" + str(i)+".txt", 'w') as file_write:
        for line in file_read:
            if p:
                line = line[:-1].split(' ')
                newLine = ""
                for word in line:
                    if word not in stopWords:
                        newLine += stem.stem(word) + " "
                file_write.write(newLine)
            elif line[0] == "\n":
                        p = True
