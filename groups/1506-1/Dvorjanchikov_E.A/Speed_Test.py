from pyparsing import *
import os;
full_module_name = Word(alphas + nums + ' ')
parse_module = full_module_name +':' + full_module_name
param = []
f = open('Configuration_test.txt')
for i in range(3):
     data = f.readline()
     param.append(parse_module.parseString(data).asList().pop(2))
f.close()
s1 = os.getcwd()
os.chdir(s1+'/1-test-version/lab1_v1/x64/Release')
start = int(param.pop(0));
step = int(param.pop(0));
count = int(param.pop(0));
ss =int(start)
for i in range(count):
	os.system('Generator_v1.exe '+str(start))
	os.system('Lab1_v1.exe')
	os.system('move Input ../../../../2-openmp/Lab2_v1/x64/Release')
	os.chdir(s1+'/2-openmp/Lab2_v1/x64/Release')
	os.system('Lab2_v1.exe')
	os.system('move Input ../../../../3-tbb/Lab1_v1/x64/Release')
	os.chdir(s1+'/3-tbb/Lab1_v1/x64/Release')
	os.system('Lab1_v1.exe')
	os.chdir(s1+'/1-test-version/lab1_v1/x64/Release')
	start = int(start)
	start += step
os.system('move time.txt ../../../../time-t.txt')
os.chdir(s1+'/2-openmp/Lab2_v1/x64/Release')
os.system('move time.txt ../../../../time-openmp.txt')
os.chdir(s1+'/3-tbb/Lab1_v1/x64/Release')
os.system('move time.txt ../../../../time-tbb.txt')
os.chdir(s1)
os.system('Show.py')