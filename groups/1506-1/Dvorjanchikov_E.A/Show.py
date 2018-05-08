# -*- coding: utf8 -*-    
import numpy as np
import matplotlib.pyplot as plt
import pylab

from pyparsing import *
import os;
full_module_name = Word(alphas + nums + ' ')
parse_module = full_module_name +':' + full_module_name
param = []
time1 = []
time2 = []
time3 = []
f = open('Configuration_test.txt')
for i in range(3):
     data = f.readline()
     param.append(parse_module.parseString(data).asList().pop(2))
	 
f.close()
start = int(param.pop(0));
step = int(param.pop(0));
count = int(param.pop(0));
param.clear()
f = open('time-t.txt')
pylab.figure(0)
for i in range(count):
	data = f.readline()
	time1.append(int(parse_module.parseString(data).asList().pop(2)))
	param.append(i*step + start)
line_10 = plt.plot(param,time1,'ro-' , label ='Test_Version') #Построение графика
f.close()
f = open('time-openmp.txt')
for i in range(count):
	data = f.readline()
	time2.append(int(parse_module.parseString(data).asList().pop(2)))
line_20 = plt.plot(param,time2,'ko-', label ='OpenMP_Version') #Построение графика
f.close()
f = open('time-tbb.txt')
for i in range(count):
	data = f.readline()
	time3.append(int(parse_module.parseString(data).asList().pop(2)))
line_30 = plt.plot(param,time3,'go-' , label ='TBB_Version') #Построение графика
f.close()
plt.legend(loc = 'best')
plt.grid(True) #Сетка
plt.savefig('res.png', format = 'png')
plt.show() #Показать график
pylab.figure('Boost')
openmp_boost =[]
tbb_boost = []
for i in range(count):
	test = time1.pop(0)
	openmp = time2.pop(0)
	tbb = time3.pop(0)
	openmp_boost.append(float(test/openmp))
	tbb_boost.append(test/tbb)
line_10 =plt.plot(param,openmp_boost, 'go-', label ='OpenMP_Version_Boost')
line_20 =plt.plot(param,tbb_boost, 'ko-', label ='OpenMP_Version_Boost')
plt.legend(loc = 'best')
plt.grid(True) #Сетка
plt.savefig('boost.png', format = 'png')
plt.show() #Показать график