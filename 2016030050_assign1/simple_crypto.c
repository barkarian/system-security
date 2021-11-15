#include "simple_crypto.h"


char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

void otp(char *plainText){
    char randomKey[strlen(plainText)];
    char encryptedText[(int)strlen(plainText)+1];//+1 for '\0' character at the end
    char decryptedText[(int)strlen(plainText)+1];

    //random byte initialization
    int byte_count = 1;
    unsigned char randomByte;
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    
    int i=0;
    while(i<strlen(plainText)){
        // while loop Initializations
        unsigned char encryptedChar=(unsigned char) 0;
        while(encryptedChar<=32||encryptedChar>=127){
            //random byte getter
            fread(&randomByte, 1, byte_count, fp);
            encryptedChar=randomByte^plainText[i];
        };
        //setting up the arrays with the random number we just found
        randomKey[i]=randomByte;
        encryptedText[i]=encryptedChar;

        i++;
    }
    randomKey[i]=(unsigned char)'\0';
    encryptedText[i]=(unsigned char)'\0';
    printf("[OTP]encrypted:%s\n",encryptedText);

    //DECRYPTIONS STARTS HERE
    for (i=0;i<strlen(plainText);i++){
        decryptedText[i]=encryptedText[i] ^ randomKey[i];
    }
    decryptedText[i]=(unsigned char)'\0';;
    printf("[OTP]decrypted:%s\n",decryptedText);

    fclose(fp);
    return;
}


char shifting(char encryptedChar,int shiftsLeft){
     encryptedChar=(unsigned char)encryptedChar;
     shiftsLeft=(unsigned char)shiftsLeft;
     while (shiftsLeft!=0){
                        //z++;                                                                        //TEST
        if(encryptedChar>=(unsigned char)48&&encryptedChar<=(unsigned char)57){
            if (encryptedChar+shiftsLeft<=(unsigned char)57){
                encryptedChar=encryptedChar+shiftsLeft;
                shiftsLeft=(unsigned char)0;
            }
            else{
                    shiftsLeft=shiftsLeft-((unsigned char)57-encryptedChar)-(unsigned char)1; //-1 stands for the
                                        //jump from numbers to UPPERCASE letters
                    encryptedChar=(unsigned char)65;
                }
            }
            else if(encryptedChar>=(unsigned char)65&&encryptedChar<=(unsigned char)90){
                  if (encryptedChar+shiftsLeft<=(unsigned char)90){
                    encryptedChar=encryptedChar+shiftsLeft;
                    shiftsLeft=(unsigned char)0;
                }
                else{
                    shiftsLeft=shiftsLeft-((unsigned char)90-encryptedChar)-(unsigned char)1;
                    encryptedChar=(unsigned char)97;
                }
            }
            else if(encryptedChar>=(unsigned char)97&&encryptedChar<=(unsigned char)122){
                 if (encryptedChar+shiftsLeft<=(unsigned char)122){
                    encryptedChar=encryptedChar+shiftsLeft;
                    shiftsLeft=(unsigned char)0;
                }
                else{
                    shiftsLeft=shiftsLeft-((unsigned char)122-encryptedChar)-(unsigned char)1; 
                    encryptedChar=(unsigned char)48;
                 }     
                }
                else{
                    //no cryptography if wrong input
                }
        
            //printf("WhileLoop %d :character=%d shiftLefts= %d\n",z,(int)encryptedChar,shiftsLeft ); //TEST
        
        }//while loop ended AND encryptedChar Propably has been founded
        return encryptedChar;
}



void caesars(char *plainText,int keyShift){
    
    char encryptedText[strlen(plainText)+1];
    char decryptedText[strlen(plainText)+1];
    char encryptedChar;
    int shiftsLeft;
    int decryptedShifts;

    if (keyShift>=62){
        keyShift=keyShift%62;
    }
    int i;
    for(int k=0;k<=1;k++){ //this for happens ones for encryptions and ones for descryption
        for (i=0;i<strlen(plainText);i++){
            if(k==0){
                encryptedChar=plainText[i];
            }
            else{
                encryptedChar=encryptedText[i];
            }
            shiftsLeft=keyShift;
                    //int z=0;                                                                        //TEST
            encryptedChar=(char)shifting(encryptedChar,(int)shiftsLeft);
             
             //printf("[FINISH]character=%c shiftLefts= %d whileLoopIn=%d\n",encryptedChar,shiftsLeft,z);  //TEST
            if(k==0){ //decrypt
                encryptedText[i]=encryptedChar;
            }else{
                decryptedText[i]=encryptedChar;
            }
        }
    
        keyShift=62-keyShift; //for second time
    }
    encryptedText[i]='\0';
    decryptedText[i]='\0';
    //printf("keyShift= %d\n",keyShift);  //TEST                                               
    printf("[Caesars]encrypted:%s\n",encryptedText);
    printf("[Caesars]decrypted:%s\n",decryptedText);
    return ;
}

int shiftingParam(char c){
    c=(unsigned char)c;
    int shiftingParam;
    if (c<=(unsigned char)57){
        shiftingParam=c-(unsigned char)48;
    }
    else if(c>=(unsigned char)65 && c<=(unsigned char)90){
        shiftingParam=c-(unsigned char)65+ 10;
    }
    else if(c>=(unsigned char)97){
        shiftingParam=c-(unsigned char)97+36 ;
    }
    //akoma kai sto 0 na eimaste dld ASCII=57 prepei na kanoume shift mia thesi
}

int shiftingParamUpperCase(char c){
    c=(unsigned char)c;
    int shiftingParam;
    shiftingParam=c-(unsigned char)65;
    //printf("character:%c,%d shiftingParam:%d \n",c,c,shiftingParam);
    return shiftingParam;
    //akoma kai sto 0 na eimaste dld ASCII=57 prepei na kanoume shift mia thesi
}

char shiftingUpperCase(char encryptedChar,int shiftsLeft){
     encryptedChar=(unsigned char)encryptedChar;
     shiftsLeft=(unsigned char)shiftsLeft;
     int temp;
                                                                       //TEST
        if (encryptedChar+shiftsLeft<=(unsigned char)90){
             encryptedChar=encryptedChar+shiftsLeft;
             return encryptedChar;
         }
         else{
            temp=(unsigned char)90-encryptedChar;//till the end and again to A
            shiftsLeft=shiftsLeft-temp-(unsigned char)1;                            //and again to A
            encryptedChar=(unsigned char)65;
            encryptedChar=encryptedChar+shiftsLeft;
         }

        
            //printf("WhileLoop %d :character=%d shiftLefts= %d\n",z,(int)encryptedChar,shiftsLeft ); //TEST
        //while loop ended AND encryptedChar Propably has been founded
        return encryptedChar;
}


void vigenere(char *plainText,char* key){
    
    char encryptedText[strlen(plainText)+1];
    char decryptedText[strlen(plainText)+1];
    char encryptedChar;
    int shiftsLeft;
    int decryptedShifts;
    int keyShift=5;


    int i;
    int ringCounter=0;        //
    int flagRC=-500;  //flagRC start again the ring counter
    for(int k=0;k<=1;k++){ //this for happens ones for encryptions and ones for descryption
        for (i=0;i<strlen(plainText);i++){
            if(k==0){
                encryptedChar=plainText[i];
            }
            else{
                encryptedChar=encryptedText[i];
            }
            

            if(flagRC!=-500){ //starting condition
            flagRC=(i)%strlen(key); //gives 0 when we must go again to the first element
            }else{
                flagRC=0;
            }  
            if(flagRC==0){
                ringCounter=0;
            }else{
                ringCounter++;
            }
            //printf("%d",ringCounter); //TEST leave it here for now
                    //int z=0;                                                                        //TEST
            

            shiftsLeft=shiftingParamUpperCase(key[ringCounter]);
            if(k==1){
                shiftsLeft=26-shiftsLeft;
            }
            // printf("shift left :%d\n",shiftsLeft);


            encryptedChar=(char)shiftingUpperCase(encryptedChar,(int)shiftsLeft);
             
             //printf("[FINISH]character=%c shiftLefts= %d whileLoopIn=%d\n",encryptedChar,shiftsLeft,z);  //TEST
            if(k==0){ //decrypt
                encryptedText[i]=encryptedChar;
            }else{
                decryptedText[i]=encryptedChar;
            }
        }
    
        flagRC=-500;                   //initialize ring counter
    }
    encryptedText[i]='\0';
    decryptedText[i]='\0';
    //printf("keyShift= %d\n",keyShift);  //TEST                                               
    printf("[Vigenere]encrypted:%s\n",encryptedText);
    printf("[Vigenere]decrypted:%s\n",decryptedText);
    return ;
}






