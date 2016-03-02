#! /bin/bash

./design 2


echo "1. putting haha into a"
echo "haha" > test/a
echo "2. putting lolol into b"
echo "lolol" > test/b
echo "3. hard link"
ln test/a test/c
echo "4. overwriting in hard linked files"
echo "damn daniel" > test/c
echo "5. symbolic link"
ln -s test/b test/d
echo "6. overwriting in symbolic linked files"
echo "damn daniel 2" > test/d


./design -1

echo "a could be haha or damn daniel"
cat test/a
echo "b could be lolol or damn daniel 2"
cat test/b
echo "c could be damn daniel"
cat test/c
echo "d could be damn daniel 2"
cat test/d
