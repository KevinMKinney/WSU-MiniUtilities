#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 512

int main(int argc, char const *argv[]) {

	// var inits
	FILE *fp = NULL;
	char *buf = calloc(BUF_SIZE, sizeof(char));
    if(buf == NULL) exit(1);     

	// for each argument...
	for (int i = 1; i < argc; i++) {
		// ... read contents if user can open file and read it
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			fprintf(stderr, "my-cat: cannot open file\n");
			free(buf);
			exit(1);
		}

		// put content in file stream on the buffer
		errno = 0;
		while (fgets(buf, BUF_SIZE, fp) != NULL) {
			// error check
			if (errno != 0) {
				fprintf(stderr, "%s\n", strerror(errno));
				free(buf);
				exit(1);
			}
			// print buffer contents
			printf("%s", buf);
		}

		fclose(fp);
	}

	free(buf);
	return 0;
}
