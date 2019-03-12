#!/bin/bash

test="test12"

./tema1 < _test/inputs/${test}.in > out
cp _test/ref/${test}.ref ref

cat out
cat ref

diff out ref

#rm out
#rm ref
