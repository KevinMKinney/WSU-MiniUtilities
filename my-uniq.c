#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 512
/**
/   This will detect if any adjecent lines in a file, fp, are duplicates,
/   and if so will 'delete' them by not sending them to stdout, while 
/   printing the rest of the lines out. It will only print one line for each
/   every duplicate adjacent lines.
**/
void myUniq(FILE *fp){
    
	char *Str1 = malloc(BUF_SIZE * sizeof(char));
	char *Str2 = malloc(BUF_SIZE * sizeof(char));
    if(Str1 == NULL || Str2 == NULL) exit(1);

    //Get the first line and print it, if there is one
    if(fgets(Str1, BUF_SIZE, fp)){
        printf("%s", Str1);
    }
    
    while( fgets(Str2, BUF_SIZE, fp) ){           
        //From the next line, compare that to the previous adjacent line, and print if different
        //Then copy and save this new line for next line
        int n = strncmp(Str1, Str2, BUF_SIZE);
        if(n != 0){
            printf("%s", Str2);
        }
        strncpy(Str1, Str2, BUF_SIZE);
    }
    free(Str1);
    free(Str2);
    //In case we get null from fgets
    if(ferror(fp)){
        printf("my-uniq: cannot read from file");
        exit(1);
    }
}

int main(int argc, char const *argv[]) {
    FILE* fp;
    
    if(argc <= 1){//In case no file is given, use stdin
        fp = stdin;
        //To end this, the user must punch in ctrl + D command
        // and my-uniq will stop work with stdin and end the program
        if(fp){
	        myUniq(fp);
        }
        else{
            printf("my-uniq: cannot open file\n");
            exit(1);
        }
    }
    

    else{//In case there are files, go through them, have to add way to continue
	    for(int i = 1; i < argc; i++){
            fp = fopen(argv[i], "r");
            if(fp){
                myUniq(fp);
                if(fclose(fp) != 0){
                    printf("my-uniq: cannot close file\n");
                    exit(1);
                }
	        }
            else{//In case we can't open a file
                printf("my-uniq: cannot open file\n");
                exit(1);
            }
        }
    }
    return 0;
}
