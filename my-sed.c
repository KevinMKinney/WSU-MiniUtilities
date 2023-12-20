#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 512

/**
/   This helper function reads a file descriptor, one character at a time, into
/	a buffer which is compared to the string we need to find. If the string was
/	found, it prints the replacing sting. Otherwise, it prints the characater that
/ 	was read.
/	Returns 1 on success or -1 on failure.
**/
int mySed(int fd, int fss, char const *fString, char const *rString, char *buf) {
	// var inits
	int fileSize = lseek(fd, 0, SEEK_END);
	int foundWordInLine = 0;

	for (int i = 0; i < fileSize; i++) {
		// move fd offset 1 char at a time
		if (lseek(fd, i, SEEK_SET) < 0) {
			return -1;
		}

		// read the necessary next chars 
		if (read(fd, buf, fss) < 0) {
			return -1;
		}

		if (foundWordInLine == 0) {
			if (strncmp(fString, buf, fss) == 0) {
				// found string
				i += (fss - 1);
				printf("%s", rString);
				foundWordInLine = 1;
			} else {
				// did not find string
				printf("%c", buf[0]);
			}
		} else {
			// already found string in line
			if (buf[0] == '\n') {
				foundWordInLine = 0;
			}
			printf("%c", buf[0]);
		}
	}
	return 1;
}

// arg 2 - what to find
// arg 3 - what to replace arg 2 with
// arg 4+ - what file(s) to run my-sed on (stdin if not specified)
// ./my-sed find-term replace-term [file...]
int main(int argc, char const *argv[]) {
	
	// checking if there is valid input
	if (argc < 3) {
		fprintf(stderr, "my-sed: find-term replace-term [file...]\n");
		exit(1);
	}

	// var inits
	int findStringSize = strlen(argv[1]);
	char *buf = calloc(findStringSize, sizeof(char));
    if(buf == NULL) exit(1);

	if (argc < 4) {
		// no file specified, sed runs on stdin
		
		/*
		// valid code if you don't read from stdin stream
		if (mySed(0, findStringSize, argv[1], argv[2], buf) < 0) {
			fprintf(stderr, "%s\n", strerror(errno));
			free(buf);
			exit(1);
		}*/

		int foundWordInLine = 0;
		int ch;

		while (1) {
			// put new character at the end of the buffer
			ch = getchar();
			for (int i = 0; i < findStringSize - 1; i++) {
				buf[i] = buf[i+1];
			}
			buf[findStringSize - 1] = ch;
			//printf("(%s | %c)", buf, ch);

			// only stop when stream ends
			if (ch == EOF) {
				break;
			}

			if (ch == '\n') {
				// print buffer, then clear it for next line
				for (int i = 0; i < findStringSize - 1; i++) {
					if (buf[i] != '\0') {
						printf("%c", buf[i]);
					}
				}
				printf("\n");
				memset(buf, 0, findStringSize * sizeof(char));
				foundWordInLine = 0;
			} else {
				// only look for string if it has not already found it
				if (foundWordInLine == 0) {
					if (strncmp(argv[1], buf, findStringSize) == 0) {
						// found string!
						memset(buf, 0, findStringSize * sizeof(char));
						printf("%s", argv[2]);
						foundWordInLine = 1;
					} else {
						printf("%c", buf[0]);
					}
				} else {
					printf("%c", buf[0]);
				}
				
			}
		}
		printf("%.*s", findStringSize - 1, buf);

	} else {
		// 1 or more files specified, sed runs on files
		int fd;
		for (int i = 3; i < argc; i++) {
			// get file descriptor of file
			fd = open(argv[i], O_RDONLY);
			if (fd < 0) {
				fprintf(stderr, "my-sed: cannot open file\n");
				free(buf);
				exit(1);
			}

			// run my-sed
			if (mySed(fd, findStringSize, argv[1], argv[2], buf) < 0) {
				fprintf(stderr, "%s\n", strerror(errno));
				free(buf);
				exit(1);
			}

			// remove file descriptor
			if (close(fd) < 0) {
				fprintf(stderr, "%s\n", strerror(errno));
				free(buf);
				exit(1);
			}
		}
	}

	free(buf);
	return 0;
}
