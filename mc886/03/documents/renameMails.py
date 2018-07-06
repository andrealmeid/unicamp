import os
import shutil

i = 0

for filename in os.listdir("docs/"):
    shutil.copyfile("docs/" + filename, "emails/email" + str(i) + ".txt")
    i += 1
