from pyparsing import *
import os;
full_module_name = Word(alphas + nums + ' ')
parse_module = full_module_name +':' + full_module_name
param = []
f = open('Configuration.txt')
for i in range(7):
     data = f.readline()
     param.append(parse_module.parseString(data).asList().pop(2))
f.close()
s1 = os.getcwd();
os.chdir(s1+'/x64/Debug')
namein = param.pop(0)
recreate = param.pop(0)
count = param.pop(0)
type = param.pop(0)
check = param.pop(0)
nameout = param.pop(0)
show = param.pop(0)

if type == 'SortDown':
    type ='3'
elif type == 'SortUp':
    type ='2'
else:
    type ='1'
if namein == 'S':
    namein ='Input'
if nameout == 'S':
    nameout ='Output'
if recreate =='Yes':
    os.system('Generator_v1.exe '+count +' '+type+' '+namein)
os.system('Lab1_v1 '+ namein+' '+nameout);
if check == 'Yes':
	os.system('Checker_v2.exe ' + namein+' '+nameout)
	f = open('result.txt')
	data = f.read()
	print(data)
	f.close()
if show == 'Yes':
	os.system('Typer_v1.exe ' + nameout+' show.txt')
	os.system('show.txt')