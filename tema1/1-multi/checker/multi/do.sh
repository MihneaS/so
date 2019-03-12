#!/bin/bash

test="myin"
args="-v --leak-check=full --show-leak-kinds=all"


valgrind $args ./tema1 $test #_test/inputs/${test}.in.1 _test/inputs/${test}.in.2 _test/inputs/${test}.in.3 _test/inputs/${test}.in.4 > out
#echo _test/inputs/${test}.in
#gdb ./tema1
cp _test/ref/${test}.ref ref

cat out
cat ref

diff out ref

rm out
rm ref
