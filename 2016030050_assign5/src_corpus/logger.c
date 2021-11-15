#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>



int file_check_write(char* path,int* action_denied,char * mode){
	int accessType;
	int retval;
	//printf("%s\n",path );
	if( (strcmp(mode,"fwrite")||strcmp(mode,"a")||strcmp(mode,"a+")||
		strcmp(mode,"w")==0||strcmp(mode,"w+")==0||
		strcmp(mode,"a")==0)	&&	access(path,F_OK)==0){			//write file
		accessType=2;
		retval=access(path,W_OK);
	}
	else if((strcmp(mode,"w")==0||strcmp(mode,"w+")==0||strcmp(mode,"a")==0)&&access(path,F_OK)!=0){
		accessType=0;	//new file
		retval=0;
	}
	else if((strcmp(mode,"a+")||strcmp(mode,"r+"))&& access(path,F_OK)==0){
		accessType=1;	//open a file
		retval=(int)(access(path,R_OK)==0&&access(path,W_OK)==0);

	}
	else{
		accessType=3;	//open a file
		retval=access(path,R_OK);
	}

	// if(accessType==2){//write file
	// 	retval=access(path,W_OK);
	// }else if(accessType==1){	//open a file
	// 	retval=access(path,R_OK);
	// }else if(accessType==0){//new file
	// 	retval=0;
	// }
	if(retval==0){
		*action_denied=0;
	}else{
		*action_denied=1;
	}
	return accessType;
}





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

void logInfos(FILE* fp,const char *path, const char *mode,int isActionDenied,int accessType) 
{
	unsigned int userId;
	char buffer[1500];
	char pathAndName[1000],date[80],timestamp[80];

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
	size_t filesize;
	unsigned char *buf;
	unsigned char *md5_result=NULL;
	if(fp!=NULL){
	fseek(fp,0L,SEEK_END);
	filesize=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	buf=(unsigned char *)malloc(sizeof(unsigned char)*filesize);
	fread(buf,filesize,sizeof(unsigned char),fp);
	md5_result=malloc(MD5_DIGEST_LENGTH);
	MD5(buf,filesize,md5_result);
		isActionDenied=0;
	}else{
		isActionDenied=1;
	}


	//Checking for user Intentions
	// if(mode==NULL){			//write file
	// 	accessType=2;
	// }
	// else if(*mode==(char)'w'){
	// 	accessType=0;	//new file
	// }
	// else{
	// 	accessType=1;	//read a file
	// }

	sprintf(buffer,"UID:%d|PATH:%s|DATE:%s|TIMESTAMP:%s|ACCESSTYPE:%d|ISACTIONDENIED:%d|FINGERPRINT:"
			,userId,pathAndName,date,timestamp,accessType,isActionDenied);
	//sprintf(buffer+strlen(buffer),"%02x",hash[md5count]);
	//sprintf(buffer+strlen(buffer),"%s\n",buf);
	//sprintf(buffer+strlen(buffer),"|\n");
	//printf("%s", bufferTmp);
	if(isActionDenied==0){
		for(int i=0;i<MD5_DIGEST_LENGTH;i++){
		sprintf(buffer+strlen(buffer),"%02x",md5_result[i]);
		}
		sprintf(buffer+strlen(buffer),"|\n");
	}else{
		sprintf(buffer+strlen(buffer),"no changes|\n");
	}

	logHandler(buffer);


}

FILE *fopen(const char *path, const char *mode) 
{	
	char noConstPath[1000],noConstMode[50];
	sprintf(noConstPath,"%s",path);
	sprintf(noConstMode,"%s",mode);
	int actionDenied;
	int accessType=file_check_write(noConstPath,&actionDenied,noConstMode);
	//printf("AApath:%s action denied:%d accessType:%d\n",noConstPath,actionDenied,accessType);
	//printf("%s\n",path );

	// if(!(strcmp(mode,"w")==0||strcmp(mode,"w+")==0)){//when creating a file access variables are not yet set
	// 	logInfos(NULL,path,mode,actionDenied);//logInfos(stream,path,NULL);
	// 	return NULL;
	// }else{
	// 	actionDenied=0;
	// }
	if(actionDenied==1){//when creating a file access variables are not yet set
		logInfos(NULL,path,mode,actionDenied,accessType);//logInfos(stream,path,NULL);
		return NULL;
	}


	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);
	
	//printf("222%s\n",noConstPath );
	logInfos(original_fopen_ret,noConstPath,mode,actionDenied,accessType);

	return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{


	char path[1000],procLink[1000];
	size_t pathSize;
	char relativePath[500];
	
	sprintf(procLink,"/proc/self/fd/%d",fileno(stream));
	pathSize=readlink(procLink,path,1000);
	path[pathSize]=(char)'\0';

	char *token=strtok(path,"/");
	while(strncmp(token,"files",5)!=0){
		token=strtok(NULL,"/");
	}
	sprintf(relativePath,"./%s",token);
	token=strtok(NULL,"/");
	sprintf(relativePath+strlen(relativePath),"/%s",token);
	//printf("%s",relativePath);
	int actionDenied;
	int accessType=file_check_write(relativePath,&actionDenied,"fwrite");
	//printf("path:%s action denied:%d accessType:%d\n",relativePath,actionDenied,accessType);

	if(actionDenied==1){
		logInfos(NULL,path,NULL,actionDenied,accessType);//logInfos(stream,path,NULL);
		return 0;
	}

	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);

	
	logInfos(stream,path,NULL,actionDenied,accessType); 


	return original_fwrite_ret;

}


