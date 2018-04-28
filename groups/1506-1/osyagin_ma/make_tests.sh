#!/bin/bash
rm -rf Tests
mkdir Tests
./test-version/gen 10 ./Tests/10
echo "10 done"
./test-version/gen 50 ./Tests/50
echo "50 done"
./test-version/gen 100 ./Tests/100
echo "100 done"
./test-version/gen 200 ./Tests/200
echo "200 done"
./test-version/gen 500 ./Tests/500
echo "500 done"
./test-version/gen 1000 ./Tests/1000
echo "1000 done"
./test-version/gen 2000 ./Tests/2000
echo "2000 done"
./test-version/gen 5000 ./Tests/5000
echo "5000 done"
./test-version/gen 10000 ./Tests/10000
echo "10000 done"
./test-version/gen 20000 ./Tests/20000
echo "20000 done"
