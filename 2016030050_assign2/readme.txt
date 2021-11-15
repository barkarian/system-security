We create just two new function for reading and writing to file we also declare a macro at the start.
	unsigned char *read_file(char *path, unsigned long *data_len)
	void write_file(char *path, unsigned char *data, unsigned long data_len)
	#define AES_ECB(bits) (bits == 128) ? EVP_aes_128_ecb() : EVP_aes_256_ecb() //new

New stuffs added to the Initial code in unexpected places (With that i mean places that they don't have the TODOs comments)have on their side this comment: //new.
Most Of the code nonetheless is inside TODOs Blocks.
Also the code has Error Handling in places that i found it neccesary for me to debug and in places that ERRORS are most common to know if we crash why and where we crash.

Task F.4.
They both don't verify.

