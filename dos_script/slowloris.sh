#!/bin/bash

for i in {1..2}; do
	(
		exec 3<>/dev/tcp/localhost/8080
		echo -n "GET / HTTP/1.1" >&3
		sleep 10
		echo -e "\r\nHost: localhost\r\n\r\n" >&3
		cat <&3
		exec 3<&-
	) &
done
wait
