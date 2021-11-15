#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>
void logHandler(char * buffer){

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)("file_logging.log", "a");


	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	size_t size=original_fwrite_ret = (*original_fwrite)(buffer, 1, strlen(buffer), original_fopen_ret);
	size++;//just to stop printing warning
	return;
}

void logInfos(FILE* fp,const char *path, const char *mode) 
{
	unsigned int userId;
	char buffer[1024];
	char pathAndName[1000],date[80],timestamp[80];
	int	accessType,isActionDenied;

	//get userId as an unsigned integer
	userId=(unsigned int)getuid(); //getuid returns a uid_t
	//get Path +stringify
	realpath(path,pathAndName);
	//get time+stringify
	time_t rawtime;
	struct tm *timeInfo;

	time(&rawtime);
	timeInfo=localtime(&rawtime);

	strftime(timestamp,80,"%T",timeInfo);//get time stringify
	strftime(date,80,"%F",timeInfo);//getdate stringify

	//opening file and create fingerprint
	char bufferTmp[1024];					//CHANGE this one read from this
	char finalHashing[1024];
	*finalHashing='\0';
	//sprintf(finalHashing,"");
	unsigned char hash[MD5_DIGEST_LENGTH];
	int md5count;
	MD5_CTX mdContext;

	if(fp){
		//timestamp hashing
		MD5_Init(&mdContext);
		while( (md5count = fread(bufferTmp,1,1024,fp))!=0 ){ //not sure if it's gonna work
			//md5count = fread(bufferTmp,1,1024,fp)==1024
			MD5_Update(&mdContext,bufferTmp,md5count);
		}
		MD5_Final(hash,&mdContext);
		isActionDenied=0;

		for(md5count=0;md5count<MD5_DIGEST_LENGTH;md5count++){
			sprintf(finalHashing+strlen(finalHashing),"%-2x",hash[md5count]);
		}
	}
	else{
		isActionDenied=1;
	}

	//Checking for user Intentions
	if(mode==NULL){			//write file
		accessType=2;
	}
	else if(*mode==(char)'w'){
		accessType=0;	//new file
	}
	else{
		accessType=1;	//read a file
	}

	sprintf(buffer,"UID:%d|PATH:%s|DATE:%s|TIMESTAMP:%s|ACCESSTYPE:%d|ISACTIONDENIED:%d|FINGERPRINT:"
			,userId,pathAndName,date,timestamp,accessType,isActionDenied);
	//sprintf(buffer+strlen(buffer),"%02x",hash[md5count]);
	sprintf(buffer+strlen(buffer),"%s\n",finalHashing);
	//sprintf(buffer+strlen(buffer),"|\n");
	//printf("%s", bufferTmp);


	logHandler(buffer);


}

FILE *fopen(const char *path, const char *mode) 
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);

	logInfos(original_fopen_ret,path,mode);

	return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{

	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);

	char path[1000],procLink[1000];
	size_t pathSize;
	
	sprintf(procLink,"/proc/self/fd/%d",fileno(stream));
	pathSize=readlink(procLink,path,1000);
	path[pathSize]=(char)'\0';
	logInfos(stream,path,NULL); //NULL signature we write


	return original_fwrite_ret;

}


