#!/bin/bash
for file in ./Tests/*; do
  if [[ "$file" != *perfect.ans ]] &&
        [[ "$file" != *.seq ]] && 
           [[ "$file" != *.omp ]] &&
             [[ "$file" != *.tbb ]];
  then
    ./test-version/b 4 $file $file".seq" $file".omp" $file".tbb"
    diff $file".seq" $file".omp"
    diff $file".seq" $file".tbb"
  fi
done
