#!/bin/bash
g++ generator.cpp -o gen
g++ typer.cpp -o t
g++ before_code.cpp -o b
g++ viewer.cpp -o v
./gen 9 t_in
./t t_in.txt t_out.bin
./b 2 t_out.bin b_out.bin
./v b_out.bin
