@off
del status.txt
start  /b /wait x64\Release\Generator_v1.exe 100 3
start /b /wait x64\Release\Lab1_v1.exe
start /b /wait x64\Release\Checker_v1.exe
del input
del output
