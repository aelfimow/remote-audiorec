CFLAGS = -mwindows -Wall -Wextra -pedantic -mthreads -D_MT -lWs2_32

all:
	gcc -c main.c -o main.o ${CFLAGS}
	gcc main.o -o remote-audiorec.exe ${CFLAGS}
