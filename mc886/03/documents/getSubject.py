import os

for filename in os.listdir("docs/"):
    with open("docs/" + filename) as file:
        for line in file:
            if line[0:7] == "Subject":
                print(line[9:], end="")
