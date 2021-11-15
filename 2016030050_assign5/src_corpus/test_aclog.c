#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define _POSIX_SOURSE
#include <sys/stat.h>
#define filesCreated 50 //+1
#define modificationsfile9 20 //+1 //prints for a user if more than 7

void createFiles(size_t X);

void multi_file_illegal_modifications(){
	char buffer[200];
	FILE *file;
	size_t bytes;
	char mode[]="0000";
	int imode;
	imode=strtol(mode,0,8);
	
	for(int j=0;j<3;j++){
		for (int i=0;i<=20;i++){
			sprintf(buffer,"./files/file_%d.txt",i);
			chmod(buffer,imode);
			file = fopen(buffer, "a");
			//printf("I am here\n");
			sprintf(buffer,"file_%d DATA %d",i,i);
			if(file!=NULL){
				bytes = fwrite(buffer, strlen(buffer), 1, file);
				fclose(file);
			}
		}
	}

}



void change_file_9_multiple_times(){
	char buffer[200];
	FILE *file;
	size_t bytes;
	// char mode[]="0666";
	// int imode;
	// imode=strtol(mode,0,8);
	// sprintf(buffer,"./files/file_9.txt");
	// chmod(buffer,imode);
	for (int i=0;i<=modificationsfile9;i++){
		//printf("I am here\n");
		sprintf(buffer,"./files/file_9.txt");
		file = fopen(buffer, "a");
		//printf("I am here\n");
		sprintf(buffer,"file_%d DATA %d",i,i);
		if(file!=NULL){
			bytes = fwrite(buffer, strlen(buffer), 1, file);
			fclose(file);
		}
	}
}


void test_lab_4_create(){  //mode is mode is 0 create file
	char buffer[200];
	FILE *file;
	size_t bytes;
	int i;
	int loop=0;
	for(i=0;i<filesCreated;i++){
		sprintf(buffer,"./files/file_%d.txt",i);//file_(0 to filesCreated)
		file = fopen(buffer, "w+");
		if(file!=NULL){
			sprintf(buffer,"file_%d DATA %d",i,i);
			bytes = fwrite(buffer, strlen(buffer), 1, file);
			fclose(file);
		}
		
	}
	return;
}

void createFiles(size_t X){
	char buffer[200];
	FILE *file;
	size_t bytes;
	for(int i=0;i<X;i++){
		sprintf(buffer,"./files/file_%d.txt",i);
		file = fopen(buffer, "w+");
		sprintf(buffer,"file_%d DATA %d",i,i);
		bytes = fwrite(buffer, strlen(buffer), 1, file);
		fclose(file);
	}
	return;
}

void encryptFile(char *fileName,char * fileData){
	size_t bytes;
	FILE *file;
	// char *token=strtok(fileName,"/");
	// 	token=strtok(NULL,"/");
	// 	token=strtok(NULL,"/");
	// 	printf(" token %s\n",token );
		//sprintf(timestamp+strlen(time));
	file = fopen(fileName, "w+");
	bytes = fwrite(fileData, strlen(fileData), 1, file);
}

int main(int argc, char *argv[]) 
{


	int ch;
	char buffer[1024];
	int X;
	char encryptedData[1024];
	while (  ( ch = getopt( argc, argv, "dc:e:s:") ) != -1  ) {
		switch (ch) {		
		case 'c':
			X=atoi(optarg);
			createFiles(X);
			break;
		case 'd':
			test_lab_4_create();//create file
			change_file_9_multiple_times();//test for modification
			multi_file_illegal_modifications();
			break;	
		case 'e':
			sprintf(encryptedData,"%s",optarg);
			//return encryptedData;
		break;	
		case 's':
			encryptFile(optarg,encryptedData);
		break;	
		default :
			break;
		}

	
	}
	argc -= optind;
	argv += optind;	
	return 0;

	
	//printf("fasgdsgasd\n");

	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */

}
