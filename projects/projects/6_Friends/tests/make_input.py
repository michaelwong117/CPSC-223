import sys

if len(sys.argv) != 2:
    print("usage: " + sys.argv[0] + " <n_lines>")
    exit(1)

n = int(sys.argv[1])

for x in range(0, n):
    print(str(x) + "," + str(x + 1))