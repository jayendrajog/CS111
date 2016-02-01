#!/command/execlineb
pipeline cat /dev/urandom "" pipeline tr -dc 'a-zA-Z0-9' "" pipeline fold -w 32 "" pipeline head -n 1M "" sort