cc=gcc

all: hw1_kmp hw1_naive

hw1_kmp: hw1_kmp.c
	$(cc) -o hw1_kmp hw1_kmp.c

hw1_naive: hw1_naive.c
	$(cc) -o hw1_naive hw1_naive.c

.PHONY: clean
clean:
	rm hw1_kmp hw1_naive
