#!/usr/bin/bash
for VAR in {1..300}
do
    nc -d localhost 8888
done
