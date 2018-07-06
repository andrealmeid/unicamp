def getFiles(path, data_size=19393):
    data = []
    for i in range(0, data_size):
        with open(path + str(i) + ".txt", 'r') as f:
            data.append(f.read())
    return data

def getData(stopwords=False, stem=None):
    data_size = 19393
    data_path = "data/"

    if not stopwords  and stem == None:
        return getFiles(data_path + "no-stem/original/original")

    if stopwords and stem == None:
        return getFiles(data_path + "no-stem/sw/sw")

    if stem == "lanc" and not stopwords:
        return getFiles(data_path + "stem/original/lanc/lanc")

    if stem == "lanc" and stopwords:
        return getFiles(data_path + "stem/sw/lanc/lanc")

    if stem == "porter" and not stopwords:
        return getFiles(data_path + "stem/original/porter/porter")

    if stem == "porter" and stopwords:
        return getFiles(data_path + "stem/sw/porter/porter")

    if stem == "snow" and not stopwords:
        return getFiles(data_path + "stem/original/snow/snow")

    if stem == "snow" and stopwords:
        return getFiles(data_path + "stem/sw/snow/snow")

    if stem == "wn" and not stopwords:
        return getFiles(data_path + "stem/original/wn/wn")

    if stem == "wn" and stopwords:
        return getFiles(data_path + "stem/sw/wn/wn")

from sklearn.feature_extraction.text import CountVectorizer
import time

data = getData(True, "lanc")

print(len(data))

counter = CountVectorizer()

tic = time.clock()

terms = counter.fit(data)

print(time.clock() - tic, "s")
