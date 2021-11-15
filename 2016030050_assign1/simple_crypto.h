#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *inputString(FILE* fp, size_t size);
void otp(char *plainText);
char shifting(char encryptedChar,int shiftsLeft);
void caesars(char *plainText,int keyShift);
int shiftingParam(char c);
int shiftingParamUpperCase(char c);
char shiftingUpperCase(char encryptedChar,int shiftsLeft);
void vigenere(char *plainText,char* key);