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

rm -rf myTest
