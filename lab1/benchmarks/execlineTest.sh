#!/command/execlineb
background {
        time -p
        redirfd -r 0 out
        redirfd -w 1 /dev/null
        getpid one sort --parallel=1
}