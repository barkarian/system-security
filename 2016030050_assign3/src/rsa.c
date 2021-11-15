#include "rsa.h"
#include "utils.h"


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
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t *sieve_of_eratosthenes(int limit, int *primes_sz){
	size_t *primes;
	int i,j;
	primes = malloc((RSA_SIEVE_LIMIT)*sizeof(size_t));
	for(i=2; i<limit; i++){
		for(j=2; j<i; j++){
			if((i % j) == 0){
				break;
			}
		}
		if(i == j){
			primes[i] = i;
		}
	}
	i=0;
	for(j=2; j<limit; j++){
		if(primes[j] != 0){
			primes[i++] = primes[j];
		}
	}
	*primes_sz=i;
	//primes = (size_t *)realloc(primes, (*primes_sz)*sizeof(size_t));
	
	return primes;
} 


/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int
gcd(int a, int b)
{
int gcd;
	for(int i=1; i <= a && i <= b; ++i)
	    {
	        // Checks if i is factor of both integers
	        if(a%i==0 && b%i==0)
	            gcd = i;
	    }
    return gcd;

}


/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t
choose_e(size_t fi_n)
{
	size_t e;

	/* TODO */
	for(size_t e=2; e<fi_n;e++){
		if(e % fi_n !=0 && gcd(e,fi_n)==1){
			return e;
		}
	}
	return 0;
}

size_t encrypt(size_t message, size_t exponent_d, size_t n){
	//msg^d=enMsg*modn
	size_t i,y;
	y=1;

	for(i=0; i<exponent_d; i++){
		y = ((y * message) % n);
	}
	size_t z=mod_inverse(y,n);
	return z;

}

/*
 * Calculates the modular inverse
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: modular inverse
 */
size_t
mod_inverse(size_t a, size_t b)
{
	/* TODO */
	a=a%b;
	for(size_t i=1;i<b;i++){
		if((a * i) %b ==1) return i;
	}
	return 0;
}



/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void
rsa_keygen(void)
{
	size_t p;
	size_t q;
	size_t n;
	size_t fi_n;
	size_t e;
	size_t d;

	/* TODO */
	int primes_sz;
	size_t * primes;
	primes=sieve_of_eratosthenes(RSA_SIEVE_LIMIT,&primes_sz) ;
	
	//pick two random prime numbers
	srand(time(NULL));
	size_t prime1=rand()%primes_sz;
	size_t prime2=rand()%primes_sz;
	p=primes[prime1];
	q=primes[prime2];

	n=p*q;
	fi_n=(p-1)*(q-1);
	e=choose_e(fi_n);
	d=mod_inverse(e,fi_n);
	//printf("p%d,q%d,n%d,fi_n%d,e%d,d%d\n",p,q,n,fi_n,e,d);
	//public key creation
	char dataIn[20];
	sprintf(dataIn,"%ld:%ld:",n,d);

	write_file("public.key",dataIn,strlen(dataIn));
	
	sprintf(dataIn,"%ld:%ld:",n,e);
	write_file("private.key",dataIn,strlen(dataIn));
}


/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_encrypt(char *input_file, char *output_file, char *key_file)
{
	/* TODO */
	FILE *fp;
	char buff[255];
	size_t n;
	size_t d;

	size_t encryptedChar;
	size_t encryptedMsg[255];
	char encryptedMsgChars[255*15]; //15 psifia to maximum ana arithmo+delimeter

	fp=fopen(key_file,"r");//change "public.key" with key_file
	fscanf(fp,"%s",buff);
	char *token=strtok(buff,":");
	n=atoi(token);
	token=strtok(NULL,":");
	d=atoi(token);
	fclose(fp);
	//get input_file Data
	fp=fopen(input_file,"r");//change "public.key" with key_file
	fgets(buff,255,fp);
	for(int i=0;i<=strlen(buff);i++){
		encryptedChar=(size_t)encrypt(buff[i], d, n);
		encryptedMsg[i]=encryptedChar;
		//printf("%c ,%c |||",buff[i] ,encryptedMsg[i]);		
	}
	//change it to store it to file
	int bytesWritten=0;
	int justWritten=0;
	for(int i=0;i<=255;i++){
		if(encryptedMsg==NULL){
			break;
		}
		if(justWritten==0&& i<0){
			break; //end of line
		}
		justWritten=sprintf(&encryptedMsgChars[bytesWritten],"%ld:",encryptedMsg[i]);
		bytesWritten=bytesWritten+justWritten;
	}
	//printf("n=%ld,d=%ld",n,d);
	write_file(output_file,encryptedMsgChars,strlen(encryptedMsgChars));



}


/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_decrypt(char *input_file, char *output_file, char *key_file)
{
	size_t n;
	size_t e;
	size_t tmp;
	size_t decryptedchar;

	FILE *fp;//leaves
	char decryptedMessage[256]; //+ \0
	char buff[255*15];
	/* TODO */
	fp=fopen("private.key","r");//change "public.key" with key_file
	fscanf(fp,"%s",buff);
	
	
	char *token=strtok(buff,":");
	n=atoi(token);
	token=strtok(NULL,":");
	e=atoi(token);
	fclose(fp);
	//printf("TESTTTTT" );
	fp=fopen("outputnew.txt","r");//change "public.key" with key_file
	fscanf(fp,"%s",buff);
	

	for(int i=0;i<255;i++){
		if(i==0){
			token=strtok(buff,":");
			decryptedchar=atoi(token);
		}else{
			token=strtok(NULL,":");
			tmp=(size_t)atoi(token);
			decryptedchar=encrypt(tmp, e, n);
		}
		decryptedMessage[i]=decryptedchar;
		i++;
		//printf("%c",tmp,(char)decryptedchar );
		printf(" atoi=%ld|decryptedchar=%c",tmp,(char)decryptedchar );
	}
	decryptedMessage[255]="\0";
	printf("\n message %s\n",decryptedMessage );
	fclose(fp);
	write_file("decryptedfilenew.txt",decryptedMessage,strlen(decryptedMessage));
}
