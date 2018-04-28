#!/bin/bash
printf "compiling generator\n"
g++ -O3 ./test-version/generator.cpp -o ./test-version/gen
printf "compiling typer\n"
g++ -O3 ./test-version/typer.cpp -o ./test-version/t
printf "compiling main program\n"
g++ -O3 ./test-version/before_code.cpp -fopenmp -ltbb -o ./test-version/b
printf "compiling viewer\n"
g++ -O3 ./test-version/viewer.cpp -o ./test-version/v
printf "compiling checker\n"
g++ -O3 ./test-version/checker.cpp -o ./test-version/c
printf "done\n"
