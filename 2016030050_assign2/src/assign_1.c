#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>

#define BLOCK_SIZE 16
#define AES_ECB(bits) (bits == 128) ? EVP_aes_128_ecb() : EVP_aes_256_ecb() //new


/* function prototypes */
void print_hex(unsigned char *, size_t);
void print_string(unsigned char *, size_t); 
void usage(void);
void check_args(char *, char *, unsigned char *, int, int);
void keygen(unsigned char *, unsigned char *, unsigned char *, int);
void encrypt(unsigned char *, int, unsigned char *, unsigned char *, 
    unsigned char *, int );
int decrypt(unsigned char *, int, unsigned char *, unsigned char *, 
    unsigned char *, int);
void gen_cmac(unsigned char *, size_t, unsigned char *, unsigned char *, int);
int verify_cmac(unsigned char *, unsigned char *);
unsigned char *read_file(char *, unsigned long *);
void write_file(char *, unsigned char *, unsigned long);


/* TODO Declare your function prototypes here... */



/*
 * Prints the hex value of the input
 * 16 values per line
 */
void
print_hex(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++) {
			if (!(i % 16) && (i != 0))
				printf("\n");
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
}


/*
 * Prints the input as string
 */
void
print_string(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++)
			printf("%c", data[i]);
		printf("\n");
	}
}


/*
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void
usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "    assign_1 -i in_file -o out_file -p passwd -b bits" 
	        " [-d | -e | -s | -v]\n"
	    "    assign_1 -h\n"
	);
	printf(
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -p    psswd   Password for key generation\n"
	    " -b    bits    Bit mode (128 or 256 only)\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -s            Encrypt+sign input and store results to output\n"
	    " -v            Decrypt+verify input and store results to output\n"
	    " -h            This help message\n"
	);
	exit(EXIT_FAILURE);
}


/*
 * Checks the validity of the arguments
 * Check the new arguments you introduce
 */
void
check_args(char *input_file, char *output_file, unsigned char *password, 
    int bit_mode, int op_mode)
{
	if (!input_file) {
		printf("Error: No input file!\n");
		usage();
	}

	if (!output_file) {
		printf("Error: No output file!\n");
		usage();
	}

	if (!password) {
		printf("Error: No user key!\n");
		usage();
	}

	if ((bit_mode != 128) && (bit_mode != 256)) {
		printf("Error: Bit Mode <%d> is invalid!\n", bit_mode);
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


/*
 * Generates a key using a password
 */
void
keygen(unsigned char *password, unsigned char *key, unsigned char *iv,
    int bit_mode)
{

	/* TODO Task A */
	if(EVP_BytesToKey(AES_ECB(bit_mode), EVP_sha1(), NULL, password,
				   strlen((const char *) password), 1, key, iv   )==0){
		fprintf(stderr,"ERROR EVP_BytesToKey:\n");
	};
	
	// EVP_BytesToKey(AES_ECB(bit_mode), EVP_sha1(), NULL, password,
	// 			   strlen((const char *) password), 1, key, iv   );
	
}


/*
 * Encrypts the data
 */
void
encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext, int bit_mode)
{
	EVP_CIPHER_CTX *ctx;
	int len;

	ctx = EVP_CIPHER_CTX_new();

	if(EVP_EncryptInit_ex(ctx, AES_ECB(bit_mode), NULL, key, NULL)==0){
		fprintf(stderr,"ERROR EVP_EncryptInit_ex:\n");
	}
	//set up cipher context 1st NULL is for default implementation 2nd NULL for iv not in use
	if(EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)==0){
		fprintf(stderr,"ERROR EVP_EncryptUpdate:\n");
	}
	//EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
	
	//encrypts plain_text_leng from input plaintext
	//MAXOFWrittenData/ciphertextmaxlenght(depends on block alignment)=plaintext_len+cipher_block_size-1
	//len=encrypted byte
	//if in and out partially overlapping returns 0 =failure
	if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)==0){
		fprintf(stderr,"ERROR EVP_EncryptFinal_ex:\n");
	}
	//EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
	//padding =>Encrypt Final data 
	EVP_CIPHER_CTX_free(ctx);

	/* TODO Task B */

}


/*
 * Decrypts the data and returns the plaintext size
 */
int
decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext, int bit_mode)
{
	int plaintext_len,len;
	plaintext_len = 0;
	EVP_CIPHER_CTX *ctx;
	ctx = EVP_CIPHER_CTX_new();
	/*TODO Task C */
	if(EVP_DecryptInit_ex(ctx, AES_ECB(bit_mode), NULL, key, NULL)==0){
		fprintf(stderr,"ERROR EVP_DecryptInit_ex:\n");
	}
	if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)==0){
		fprintf(stderr,"ERROR EVP_DecryptUpdate:\n");
	}
	//The parameters and restrictions are identical to the encryption operations except that if padding is enabled the decrypted data buffer out passed to EVP_DecryptUpdate() should have sufficient room for (inl + cipher_block_size) bytes unless the cipher block size is 1 in which case inl bytes is sufficient.
	plaintext_len = len; //len=lenght of all blocks except last
	if(EVP_DecryptFinal_ex(ctx, ciphertext + len, &len)==0){
		fprintf(stderr,"ERROR EVP_DecryptFinal_ex:\n");
	}
	plaintext_len += len; //len=lenght of last block (with padding)

	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}


/*
 * Generates a CMAC
 */
void
gen_cmac(unsigned char *data, size_t data_len, unsigned char *key, 
    unsigned char *cmac, int bit_mode)
{

	/* TODO Task D */
	CMAC_CTX *ctx;
	size_t len;
	ctx = CMAC_CTX_new();
	int a,b,c;
	a=CMAC_Init(ctx, key, bit_mode / 8, AES_ECB(bit_mode), NULL);
	//keylenght in Bytes=bit_mode / 8
	b=CMAC_Update(ctx, data, data_len);
	//process data_len inputs and encrypt those
	c=CMAC_Final(ctx, cmac, &len);
	//caller must provide enough size
	if(a==0||b==0||c==0){
		fprintf(stderr,"ERROR gen_cmac:(most of the times memory is exhausted)\n");

	}
	CMAC_CTX_free(ctx);

}


/*
 * Verifies a CMAC
 */
int
verify_cmac(unsigned char *cmac1, unsigned char *cmac2)
{
	/* TODO Task E */
	int verify;
	verify = (memcmp(cmac1, cmac2, BLOCK_SIZE) == 0);
	return verify;
}



/* TODO Develop your functions here... */
/*We use two functions one for read and one for write to file*/

unsigned char *read_file(char *path, unsigned long *data_len) {
	FILE *f;
	unsigned char *data;
	
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

void write_file(char *path, unsigned char *data, unsigned long data_len) {
	FILE *f;

	f = fopen (path, "wb");
	fwrite(data, 1, data_len, f);
}



/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with 
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int
main(int argc, char **argv)
{
	int opt;			/* used for command line arguments */
	int bit_mode;			/* defines the key-size 128 or 256 */
	int op_mode;			/* operation mode */
	char *input_file;		/* path to the input file */
	char *output_file;		/* path to the output file */
	unsigned char *password;	/* the user defined password */
	/* Data Manipulation */ //NEW
	unsigned char *input_data;	/* data from input */
	unsigned long input_len;	/* length of input */
	unsigned char *output_data;	/* data for output */
	unsigned long output_len;	/* length of output */
	unsigned char tmp_cmac[BLOCK_SIZE];

	/* Init arguments */
	input_file = NULL;
	output_file = NULL;
	password = NULL;
	bit_mode = -1;
	op_mode = -1;

	/*New Init*/
	input_data = NULL; 	//new
	input_len = 0;		//new
	output_data = NULL;	//new
	output_len = 0;		//new

	unsigned char key[256], iv[256];
	/*
	 * Get arguments
	 */
	while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:")) != -1) {
		switch (opt) {
		case 'b':
			bit_mode = atoi(optarg);
			break;
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'p':
			password = (unsigned char *)strdup(optarg);
			break;
		case 'd':
			/* if op_mode == 1 the tool decrypts */
			op_mode = 1;
			break;
		case 'e':
			/* if op_mode == 1 the tool encrypts */
			op_mode = 0;
			break;
		case 's':
			/* if op_mode == 1 the tool signs */
			op_mode = 2;
			break;
		case 'v':
			/* if op_mode == 1 the tool verifies */
			op_mode = 3;
			break;
		case 'h':
		default:
			usage();
		}
	}


	/* check arguments */
	check_args(input_file, output_file, password, bit_mode, op_mode);



	/* TODO Develop the logic of your tool here... */

	/*Reading the File*/
	if (input_file != NULL){
		input_data = read_file(input_file, &input_len);
	}

	/* Initialize the library */
	/* Create Key from password */
	keygen(password, key, iv, bit_mode); 

	switch (op_mode) {
	/* Operate on the data according to the mode */
	/* encrypt */
	case 0:
		//allocate enough space = input length + padding + CMAC
		output_len = input_len + BLOCK_SIZE- (input_len % BLOCK_SIZE) ;
		output_data = malloc(output_len);

		encrypt(input_data, input_len, key, iv, output_data, bit_mode);
		break;
	/* decrypt */
	case 1:
		output_data = malloc(input_len);
		output_len = decrypt(input_data, input_len, key, iv, output_data,
				bit_mode);
		break;
	/* sign */
	case 2:
		output_len = input_len - (input_len % BLOCK_SIZE) + 2*BLOCK_SIZE;
		output_data = malloc(output_len);

		encrypt(input_data, input_len, key, iv, output_data, bit_mode);
		gen_cmac(input_data, input_len, key, output_data + (output_len - BLOCK_SIZE), bit_mode);
		break;
	/* verify */
	case 3:
		output_data = malloc(input_len);
		output_len = decrypt(input_data, input_len - BLOCK_SIZE, key, iv, output_data,
							bit_mode);

		gen_cmac(output_data, output_len, key, tmp_cmac, bit_mode);

		if (!verify_cmac(tmp_cmac, input_data + (input_len - BLOCK_SIZE)))
			return 1;
		break;
	default:
		break;
	}

	if (output_len){
		write_file(output_file, output_data, output_len);
	}

	free(input_data);
	free(output_data);

	/* Clean up */
	free(input_file);
	free(output_file);
	free(password);


	/* END */
	return 0;
}
