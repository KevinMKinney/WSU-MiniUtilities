.SUFFIXES: .c .o
CCFLAGS = -std=c99 -pedantic -Wall -Werror
OPTIONS = -g

build:
	gcc ${CCFLAGS} ${OPTIONS} -o my-cat my-cat.c
	gcc ${CCFLAGS} ${OPTIONS} -o my-sed my-sed.c
	gcc ${CCFLAGS} ${OPTIONS} -o my-uniq my-uniq.c

clean:
	rm -f my-cat my-sed my-uniq

# to run valgrind/check for memory problems run the following:
# valgrind --track-origins=yes --leak-check=full ./"my-file-name" "args..."