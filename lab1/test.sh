#! /bin/bash

function should_fail() {
	result=$?;
	
	if [ $result -lt 1 ];	# 0 for success
	then
		echo -en "\e[91mError:\e[39m\e[49m ";	# e for backslash escape, n for surpress newline
		echo "$1";	# The first arg passed in
		#exit 1;
	else
		echo -en "\e[92mPassed:\e[39m\e[49m ";
		echo "$1";
	fi
}

function should_succeed() {
	result=$?;
	
	if [ $result -gt 0 ];	# 1 for fail
	then
		echo -en "\e[91mError:\e[39m\e[49m ";	# e for backslash escape, n for surpress newline
		echo "$1";	# The first arg passed in
		#exit 1;
	else
		echo -en "\e[92mPassed:\e[39m\e[49m ";
		echo "$1";
	fi
}


./simpsh --rdonly mrjogisreallyawesome 2>&1 | grep "Cannot open" > /dev/null
should_succeed "Program reports missing files";

./simpsh --wronly mrjogisreallyawesome 2>&1 | grep "Cannot open" > /dev/null
should_succeed "Program reports missing files";

./simpsh --rdonly Makefile | grep "Cannot open" > /dev/null;
should_fail "Program is silenced when file can be successfully opened"

./simpsh --rdonly Makefile | grep "Cannot open" > /dev/null;
should_fail "Program is silenced when file can be successfully opened"

./simpsh --verbose --command 1 2 3 echo foo 2>&1 | grep "Bad file descriptor" > /dev/null
should_succeed "Program should report error when using nonexistent fd"

mkdir myTest
touch myTest/test1.txt
touch myTest/test2.txt
touch myTest/test3.txt
echo "hello" >> myTest/test1.txt
echo "xyz" >> myTest/test1.txt
echo "def" >> myTest/test1.txt
echo "true" >> myTest/test1.txt
echo "abd" >> myTest/test1.txt

./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --command 0 1 2 sort 2>&1 | grep "Bad file descriptor" > /dev/null
should_succeed "Program should report error when using nonexistent fd"

./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --wronly myTest/test3.txt --command 0 1 2 2>&1 | grep "Missing operands" > /dev/null
should_succeed "Program should report error when --command is missing operands"

./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --wronly myTest/test3.txt --command 0 1 2 sort
should_succeed "Testing sort command"

head -n 1 myTest/test2.txt | grep "abd" > /dev/null
should_succeed "Testing sort command"

tail -n 1 myTest/test2.txt | grep "xyz" > /dev/null
should_succeed "Testing sort command"


#pipe testing
#clear file contents
> myTest/test1.txt
> myTest/test2.txt
> myTest/test3.txt
./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --wronly myTest/test3.txt --pipe --command 0 4 2 echo "Chris Harris Jr is the most underrated CB in the NFL" --command 3 1 2 tr a-z A-Z
cat myTest/test2.txt | grep "CHRIS HARRIS JR IS THE MOST UNDERRATED CB IN THE NFL" > /dev/null
should_succeed "Pipe with echo and tr works"

echo "hello" >> myTest/test1.txt
echo "xyz" >> myTest/test1.txt
echo "def" >> myTest/test1.txt
echo "true" >> myTest/test1.txt
echo "abd" >> myTest/test1.txt
./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --wronly myTest/test3.txt --pipe --pipe --command 0 4 2 sort --command 3 6 2 cat - - --command 5 1 2 tr a-z A-Z
cat myTest/test2.txt | grep "TRUE" > /dev/null
should_succeed "Double piping with sort and tr works"

echo "this is anthony" > myTest/test1.txt
echo "cs111 is so fun" >> myTest/test1.txt
echo "more gibberish stuff" >> myTest/test1.txt
> myTest/test2.txt
> myTest/test3.txt
./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --wronly myTest/test3.txt --pipe --pipe --command 0 4 2 tr a-z A-Z --command 3 6 2 tr A-Z a-z --command 5 1 2 cat
diff myTest/test1.txt myTest/test2.txt > /dev/null
should_succeed "Double piping with tr and tr should work (1)"

> myTest/test2.txt
./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --wronly myTest/test3.txt --pipe --pipe --command 3 6 2 tr A-Z a-z --command 0 4 2 tr a-z A-Z --command 5 1 2 cat
diff myTest/test1.txt myTest/test2.txt > /dev/null
should_succeed "Double piping with tr and tr should work (2-switching the ordering of --command should still work)"

> myTest/test2.txt
./simpsh --rdonly myTest/test1.txt --wronly myTest/test2.txt --wronly myTest/test3.txt --pipe --pipe --command 5 1 2 cat --command 3 6 2 tr A-Z a-z --command 0 4 2 tr a-z A-Z
diff myTest/test1.txt myTest/test2.txt > /dev/null
should_succeed "Double piping with tr and tr should work (3-switching the ordering of --command should still work)"

#Let's test file flags
> myTest/test1.txt
> myTest/test2.txt
> myTest/test3.txt

echo "Don't delete me please" > myTest/test1.txt
./simpsh --rdonly myTest/test2.txt --append --wronly myTest/test1.txt --wronly myTest/test3.txt --command 0 1 2 echo "Add me please"
cat myTest/test1.txt | grep "Don't delete me please" > /dev/null
should_succeed "Testing --append"

./simpsh --rdonly myTest/test2.txt --creat --wronly myTest/test4.txt --wronly myTest/test3.txt --command 0 1 2 echo "I was just born"
cat myTest/test4.txt | grep "I was just born" > /dev/null
should_succeed "Testing --creat"

./simpsh --trunc --wronly myTest/test4.txt
cat myTest/test4.txt | grep "I was just born" > /dev/null
should_fail "Testing --trunc"


rm -rf myTest
