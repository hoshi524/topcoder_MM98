import subprocess

subprocess.call(
    'g++-5 -std=gnu++1y -O2 -o out/main.out src/main.cpp', shell=True)
subprocess.call('javac -d out src/PrincessesAndMonstersVis.java', shell=True)

for seed in range(1, 10):
    print(seed)
    subprocess.call(
        'java -cp out PrincessesAndMonstersVis -exec out/main.out -seed {0} -vis'.format(seed), shell=True)
