#!/bin/bash

make clean > /dev/null;
if [ $? -ne 0 ]; then
	echo "Failed to clean up"
	exit 1
fi
echo "cleaned old source files"

make > /dev/null;
if [ $? -ne 0 ]; then
	echo "Failed to build indexer and index test"
	exit 1
fi
echo "made index and indextest"

indexer "$1" "old" || exit 2
echo "ran indexer"
echo "first 3 lines to verify:"
head -3 old

indextest "old" "new" || exit 3
echo "ran indextest"
echo "first 3 lines to verify:"
head -3 new

echo "sorting and comparing files"
sort "old" > "oldsorted"
sort "new" > "newsorted"
diff "newsorted" "newsorted"

echo "if no output, we passed the test!"

rm old* new*
make clean > /dev/null
echo "cleaned up"
