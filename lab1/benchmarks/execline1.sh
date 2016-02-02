#!/command/execlineb
#pipeline cat /dev/urandom "" pipeline tr -dc 'a-zA-Z0-9' "" pipeline head -c 10M "" grep 'Anthony'

#redirfd -w 1 a
#pipeline { tr a-z A-Z }
#pipeline { echo "hello" }
#cat

piperw 7 8
piperw 9 10
piperw 11 12

background {
	time -p
	fdclose 7
	fdclose 9
	fdclose 10
	fdclose 11
	fdclose 12
	fdmove 1 8
	cat /dev/urandom
}

background {
	time -p
	fdmove 0 7
	fdmove 1 10
	fdclose 8
	fdclose 9
	fdclose 11
	fdclose 12
	tr -dc 'a-zA-Z0-9'
}

background {
	time -p
	fdmove 0 9
	fdmove 1 12
	fdclose 7
	fdclose 8
	fdclose 10
	fdclose 11
	head -c 10M
}

#wait { child1 }

fdmove 0 11
fdclose 7
fdclose 8
fdclose 9
fdclose 10
fdclose 12
grep 'Anthony'
	
