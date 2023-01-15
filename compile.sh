gcc src/circle.c -lbmp -lm -o bin/circle
gcc -o bin/master src/master.c -Iinclude -lncurses -lm
gcc -o bin/processA src/processA.c -Iinclude -lncurses
gcc -o bin/processB src/processB.c -Iinclude -lncurses