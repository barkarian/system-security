#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{
	int i;
	size_t bytes;
	FILE *file;
	char filenames[10][7] = {"file_0", "file_1", 
			"file_2", "file_3", "file_4",
			"file_5", "file_6", "file_7", 		
			"file_8", "file_9"};


	/* example source code */

	for (i = 0; i < 10; i++) {

		file = fopen(filenames[i], "w+");
		if (file == NULL) 
			printf("fopen error\n");
		else {
			setuid((uid_t)1234);
			bytes = fwrite(filenames[i+1], strlen(filenames[i]), 1, file);
			fclose(file);
		}

	}


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


}
