We call one function for every cipher that's responsible to encrypt and decrypt.
So every cipher has only one function that process encryption and decryption.

EMBEDDED FUNCTIONS
Vigenere embedded functions.
	->int shiftingParamUpperCase(char c); Gets a char and return the shiftingParam for that char.
	->char shiftingUpperCase(char encryptedChar,int shiftsLeft); Gets a value encryptedChar that its actually not encrypted yet and after processing returns the encrypted value for that char.

Caesars embedded functions.
	->char shifting(char encryptedChar,int shiftsLeft); Gets a value encryptedChar that its actually not encrypted yet and after processing returns the encrypted value for that char.

OTHER FUNCTIONS
	->char *inputString(FILE* fp, size_t size); Responsible to scan and store the input of the user