CS 460: Project 1 - Mini Utilities

2/8/23

Created by Kevin Kinney and Joseph Wiedeman

How to run:
	Run "make" (with provided Makefile) to build executables of all utilities. Running "make clean" removes the executables. Each of the executables require input as formated below:
	
	./my-cat [filename(s) seperated with a space]
	./my-sed [find-term] [replace-term] [filename(s) seperated with a space - if not specified, stdin will be taken as input]
	./my-uniq [filename(s) seperated with a space]

What code does:
	Each executable has similar functionality to its UNIX utility counterpart (ex: my-cat and cat). Below is a short summary of the functionality:

	my-cat; Reads all files given and prints their contents. An error will occur if no file was specified or it cannot open a file.

	my-sed: Assuming one or more files was given as input, my-sed will read then print the files' contents but replace the first instance of the find-term with the replace-term in each line. If no file was given as input, my-sed will do the same but on stdin. An error will occur if it cannot read a file or it was not given enough inputs.

	my-uniq: It will go through individual files, and check to see if any adjacent lines are the same. If they are, then they will be 'deleted', by not being printed. Only unique lines will be printed to stdout. If no files are given, then my-uniq will read from stdin until it reaches an eof, by the user hitting ctrl+D. An error will occur if it cannot read a file.

Code implementation decisions:
    Below is a short summary of the implementation decisions made for each executable:

    my-cat: The functionality of cat is straigtforward; read a file's contents into a buffer, then print the buffer. The more interesting decisions are how big to make the buffer and error handling. The buffer's original size was 128 bytes, but was later changed to a larger 512 bytes. This is mainly for the performance reasons. As for the error handling, the function fgets() return NULL on error or when it reaches the end of file character. Since these outcomes require different responses, the errno library needed to be used to determine the difference.

    my-sed: Instead of getting file contents through a file pointer, it was implemented around file descriptors. This was done to use the lseek function, which was used to move through files, get the file size, and skip over the word that needs to be replaced. However, this implementation became an issue because it did not allow for stdin to be streamed in (the code to do this is commented in the file). To solve this, the getchar function was used to get one character at a time, similar to the lseek functionality. But since it is unknown if this character is a part of the string to be found, it needs to store that character in a last in, first out data structure. This stucture is typically a stack, but due to a combination of laziness and expecting that most find strings inputs are on the smaller size, a string buffer that swaps characters was used. This does have the byproduct of making user I/O somewhat illegable, but the correct output is shown if you exclusively view stdout.

    my-uniq: The implementation decisions of this utility started with figuring out when stdin will be where input comes from or if files are. Because the stdin and the file for this utility are a bit different, it was decided to seperate just the starts of them, but keep the same idea for the uniq part of it. For the stdin section, it was decided to just keep one file pointer being stdin and put it into the myUniq function, since we never need to close stdin. Once it's done, we just return 0. While for the files, it was decided to implement where it would loop through all the files, opening them, calling myUniq for each, and closing them after we finished running the function, with error checking. The implement decision for the myUniq function was to have it go line by line, checking to see if the previous line is the same as the last one, and if it isn't, it will print it. It was decided to use the string.h library's strncmp in order to compare the adjacent lines to see if they are the same and to be printed out. The strncpy function was used as well to save the compared line in order to compare it to the next adjacent line. Also, with the use of strncmp, it won't care about newlines, so if the last two lines are the same besides the newline character, it will be considered the same line and not print. By implementing these decisions, my-uniq was able to work very similar to uniq.
