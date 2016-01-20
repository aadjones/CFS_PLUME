import fileinput
import subprocess

def findReplace(filename, toReplace, replacement):
    for line in fileinput.input(filename, inplace=True):
        print line.replace(toReplace, replacement),

def getLine(filename, expr):
    cmd = "cat " + filename + " | " + "grep " + expr
    percentLine = subprocess.check_output(cmd, shell=True)
    return percentLine

################################################################################
filename = "./cfg/stam.64.cfg"
expr = "percent"
compressCmd = "./bin/compressMatrices " + filename
compareCmd = "./bin/reducedCompressed3DCompareVelocities " + filename

percentOptions = [0.999, 0.995, 0.99, 0.985, 0.975]

for i in range(len(percentOptions)):
    toReplace = getLine(filename, expr)
    replacement = "percent = " + str(percentOptions[i]) + "\n"
    findReplace(filename, toReplace, replacement)
    subprocess.call(compressCmd, shell=True)
    subprocess.call(compareCmd, shell=True)
