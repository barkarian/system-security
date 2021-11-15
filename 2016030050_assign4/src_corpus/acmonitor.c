#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct entry {

	int uid; /* user id (positive integer) */
	int access_type; /* access type values [0-2] */
	int action_denied; /* is action denied values [0-1] */

	char date[60]; /* file access date */
	char time[60]; /* file access time */

	char file[1024]; /* filename (string) */
	char fingerprint[1024]; /* file fingerprint */
	//int malicious_count;//NEWCODE

	/* add here other fields if necessary */
	/* ... */
	/* ... */

};

struct user {
	int uid; /* user id (positive integer) */
	int malicious_count;
};

struct file {
	//char filename[1000];
	int * uid; /* user id (positive integer) */
	int * modifications;
	size_t index;
	char latestFingerprint[1000];
};


void
usage(void)
{
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
		   "Options:\n"
		   "-m, Prints malicious users\n"
		   "-i <filename>, Prints table of users that modified "
		   "the file <filename> and the number of modifications\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}

//HELPING FUNCTIONS - ADDED
int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

char * getParam(char * data){
	char *token=strtok(data,":");
	token=strtok(NULL,":");
	return token;
	//return data;
}
struct entry readLine(char * logData){
	
	char uid[20];
	char pathAndName[1000],date[80],timestamp[80];
	char accessType[20],isActionDenied[20];
	char fingerprint[1000];

	char *token=strtok(logData,"|");//uid
	sprintf(uid,"%s",token);
	token=strtok(NULL,"|");//path
	sprintf(pathAndName,"%s",token);
	token=strtok(NULL,"|");//date
	sprintf(date,"%s",token);
	token=strtok(NULL,"|");//time
	sprintf(timestamp,"%s",token);	
	token=strtok(NULL,"|");//accesstype
	sprintf(accessType,"%s",token);
	token=strtok(NULL,"|");//isactiondenied
	sprintf(isActionDenied,"%s",token);
	token=strtok(NULL,"|");//fingerprint
	sprintf(fingerprint,"%s",token);
	
	struct entry logInfo;
	logInfo.uid=atoi(getParam(uid));
	logInfo.access_type=atoi(getParam(accessType));
	logInfo.action_denied=atoi(getParam(isActionDenied));
	
	sprintf(logInfo.file,"%s",getParam(pathAndName));
	sprintf(logInfo.fingerprint,"%s",getParam(fingerprint));
	//for now date and time are strings FIX IT IF NEEDED IN NEXT LABS
	sprintf(logInfo.date,"%s",getParam(date));
	sprintf(logInfo.time,"%s",getParam(timestamp));

	//EXTRA
	return logInfo;
}

struct entry * getLogsTable(char * logData,int * sizeOfArray){
	struct entry * logInfoTable=malloc(sizeof(struct entry));
	size_t logIndex=0;
	
	size_t curLineIndex=0;
	size_t stringLenght=strlen(logData);
	for(int i=0;i<=stringLenght;i++){
		if(logIndex!=0){
			logInfoTable=(struct entry *)realloc(logInfoTable,sizeof(struct entry)*(logIndex+1));
		}
		if(logData[i]=='\n'){
			logInfoTable[logIndex]=readLine(&logData[curLineIndex]);
			//printf("Just Arrived %d,%s,%s,%s,%d,%d,%s\n",logInfo.uid,logInfo.file,logInfo.date,logInfo.time,logInfo.access_type,logInfo.action_denied ,logInfo.fingerprint);//DELETEME
			i=i+1;
			curLineIndex=i;
			logIndex++;
		}
	}
	*sizeOfArray=logIndex;
	return logInfoTable;
}

char *read_file(char *path, unsigned long *data_len) {
	FILE *f;
	char *data;
	
	f = fopen (path, "rb");
	fseek (f, 0, SEEK_END);
	*data_len = ftell(f);
	fseek (f, 0, SEEK_SET);
	data = malloc(*data_len);
	if (data){
		fread(data, 1, *data_len, f);
	}

	fclose(f);
	return data;
}



void 
list_unauthorized_accesses(char *log)
{
	unsigned long sizeOfString ;
	char * logData=read_file(log,&sizeOfString);

	int sizeOfArray;
	struct entry * logInfoTable = getLogsTable(logData,&sizeOfArray);
	struct user * usersTable=malloc(sizeof(struct user));
	size_t indexUser=-1;//points to the next index that's gonna be written
	int found;
	
	for(int i=0;i<sizeOfArray;i++){
		if(logInfoTable[i].action_denied==1){//denied log
			if(indexUser==-1){
				indexUser++;
				usersTable[indexUser].uid=logInfoTable[i].uid;
				usersTable[indexUser].malicious_count=1;
				//printf("%d %d\n",usersTable[indexUser].uid,usersTable[indexUser].malicious_count );
			}
			else{
				//printf("\nINDEX%d\n",indexUser );
				for(int j=0;j<indexUser+1;j++){//search in current users
					//printf("|uid %d,uidlog: %d,j: %d\n",usersTable[j].uid,logInfoTable[i].uid,j);
					if(usersTable[j].uid==logInfoTable[i].uid){//if there is a user with history
						//printf("OK\n");
						usersTable[j].malicious_count++;
						found=1;
						//break;
					}
				}
				if(found==0){
					indexUser++;
					usersTable=(struct user *)realloc(usersTable,sizeof(struct user)*(indexUser+1));
					usersTable[indexUser].uid=logInfoTable[i].uid;
					usersTable[indexUser].malicious_count=1;
				}
			}
			found=0;
		}
	}
	
	for(int i=0;i<indexUser+1;i++){
		if(usersTable[i].malicious_count>=7)
			printf("UID:%d\n",usersTable[i].uid);
	}
	//ALL WITH MALICIOUS COUNT JUST FOR TESTING
	// printf("\nALL WITH MALICIOUS COUNTER JUST FOR TESTING\n");
	// for(int i=0;i<indexUser+1;i++){
	// 		printf("UID:%d MC:%d\n",usersTable[i].uid, usersTable[i].malicious_count);
	// }

	return;
	//printf("Just Arrived %d,%s,%s,%s,%d,%d,%s\n",logInfoTable[i].uid,logInfoTable[i].file,logInfoTable[i].date,logInfoTable[i].time,logInfoTable[i].access_type,logInfoTable[i].action_denied ,logInfoTable[i].fingerprint);//DELETEME

}


void
list_file_modifications(char *log, char *file_to_scan)
{
	unsigned long sizeOfString ;
	char * logData=read_file(log,&sizeOfString);

	int sizeOfArray;
	struct entry * logInfoTable = getLogsTable(logData,&sizeOfArray);
	struct file fileInfos;
	fileInfos.uid=(int*)malloc(sizeof(int)*sizeOfArray);
	fileInfos.modifications=(int*)malloc(sizeof(int)*sizeOfArray);
	fileInfos.index=-1;
	int found=0;
	
	for(int i=0;i<sizeOfArray;i++){
		if(strcmp(file_to_scan,logInfoTable[i].file)==0 && strcmp(fileInfos.latestFingerprint,logInfoTable[i].fingerprint)!=0 ){
		//if(strcmp(file_to_scan,logInfoTable[i].file)==0 ){//AND strcmp (fingerprint1,fingerprint2)!=0
			if(fileInfos.index==-1){
				fileInfos.index++;
				fileInfos.uid[fileInfos.index]=logInfoTable[i].uid;
				fileInfos.modifications[fileInfos.index]=0;//for first time we don't have a way to know if it's just modify or not
				sprintf(fileInfos.latestFingerprint,"%s",logInfoTable[i].fingerprint);
				found=0;
			}
			else{
				for(int j=0;j<fileInfos.index+1;j++){
					if(fileInfos.uid[j]==logInfoTable[i].uid){
						//printf("FOR I am inside %d,%d\n",fileInfos.uid[j],fileInfos.modifications[i]);
						fileInfos.modifications[j]++;
						found=1;
						sprintf(fileInfos.latestFingerprint,"%s",logInfoTable[i].fingerprint);
						//break;
					}
				}
				if(found==0){
					fileInfos.index++;
					fileInfos.uid[fileInfos.index]=logInfoTable[i].uid;
					fileInfos.modifications[fileInfos.index]=1;
					sprintf(fileInfos.latestFingerprint,"%s",logInfoTable[i].fingerprint);
					//printf("IF2 uid:%d modifications:%d\n", fileInfos.uid[fileInfos.index],fileInfos.modifications[fileInfos.index]);//TESTME	
				}

			}
			found=0;
		}
	}

	for(int i=0;i<fileInfos.index+1;i++){
			printf("UID:%d MODIFICATIONS:%d\n",fileInfos.uid[i],fileInfos.modifications[i]);
	}
	return;
}


int 
main(int argc, char *argv[])
{

	int ch;
	char pathAndName[1000];
	if (argc < 2)
		usage();
	//FILE *log;
	//log = fopen("./file_logging.log", "r");
	char log[]="./file_logging.log";

	if (log == NULL) {
		printf("Error opening log file \"%s\"\n", "./log");
		return 1;
	}

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			realpath(optarg,pathAndName);
			list_file_modifications(log, pathAndName);
			break;
		case 'm':
			list_unauthorized_accesses(log);
			break;
		default:
			usage();
		}

	}


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	//fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}
