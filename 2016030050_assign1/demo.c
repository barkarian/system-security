//#include "simple_crypto.c"
#include "simple_crypto.h"



int main(int argc, char const *argv[])
{
	char *plainText;
	char *upperCaseText;
	//char upperCaseText[1000];
	char *stringKey;
	//char stringKey[1000];
	int key;
	
	//OTP
    printf("[OTP]input:");
    plainText = inputString(stdin, 10);
	otp(plainText);
	free(plainText);

	//Caesars
	printf("[Caesars]input:");
    plainText = inputString(stdin, 10);
    printf("[Caesars]key:");
    scanf("%d",&key);
	char c=getchar();
	caesars(plainText,key); //3rd variable ==false for encrypt ==true for decrypt
	free(plainText);
	

	//Vigenere
	printf("[Vigenere]input:");
    //scanf("%s",upperCaseText);
    upperCaseText = inputString(stdin, 10);
    printf("[Vigenere]key:");
    //scanf("%s",stringKey);
    stringKey = inputString(stdin, 10);
    vigenere(upperCaseText,stringKey);
    //printf("plainText:%s key:%s\n",plainText,stringKey  );
	//free(plainText);

	return 0;
}