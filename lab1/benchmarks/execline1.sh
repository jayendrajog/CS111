#!/command/execlineb
pipeline cat /dev/urandom "" pipeline tr -dc 'a-zA-Z0-9' "" pipeline head -c 10M "" grep 'Anthony'
