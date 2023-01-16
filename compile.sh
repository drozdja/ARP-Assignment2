#! /usr/bin/bash

mkdir -p /bin
gcc src/master.c -Iinclude -lncurses -lbmp -lrt -lm -o bin/master
gcc src/processA.c -Iinclude -lncurses -lbmp -lrt -lpthread -o bin/processA
gcc src/processB.c -Iinclude -lncurses -lbmp -lrt -lpthread -o bin/processB