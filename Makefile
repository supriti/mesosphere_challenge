CC = g++
CFLAGS = -std=c++11 -O3

all: scheduler

.PHONY = run

scheduler: scheduler.cpp
	$(CC) $(CFLAGS) -o $@ $^

run:
	./scheduler input.txt

clean:
	rm -rf scheduler *.o
