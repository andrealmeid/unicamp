import os

for filename in os.listdir("docs/"):
        with open("docs/" + filename) as file:
                for line in file:
                        if line[0:10] == "Newsgroups":
                            print(line[12:], end="")
                                break

