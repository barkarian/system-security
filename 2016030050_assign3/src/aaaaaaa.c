#include "rsa.h"
#include "utils.h"

/*
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t *
sieve_of_eratosthenes(int limit, int *primes_sz)
{
	size_t *primes;
	int i,j;

	*primes_sz = 0;
	primes = malloc((limit)*sizeof(size_t));

	primes[0] = 0;
	primes[1] = 1;

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
			(*primes_sz)++;
		}
	}

	primes = (size_t *)realloc(primes, (*primes_sz)*sizeof(size_t));
	
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
	if (a == 0){
		return b;
	}
	if(b == 0){
		return a;
	}
	return gcd(b, a % b);
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

	for(e = 2; e < fi_n; e++){
		if( ((e % fi_n) != 0) && (gcd(e,fi_n) == 1) ){
			break;
		}
	}

	return e;
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
	size_t inverse;
	a %= b;

	for(inverse=1; inverse<b; inverse++){
		if( ((a*inverse) % b) == 1 ){
			return inverse;
		}
	}

	return 0;
}


size_t mod_exponent(size_t msg, size_t exp, size_t n){

	size_t i,j;
	j=1;

	if(n == 1){
		return 0;
	}

	for(i=0; i<exp; i++){
		j = (j * msg) % n;
	}

	return j;

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

	int prime1, prime2, size;
	int primes_sz;
	size_t *primes;
	unsigned char *public_key;
	unsigned char *private_key;

	/* TODO */
	public_key = malloc(2*sizeof(unsigned char));
	private_key = malloc(2*sizeof(unsigned char));

	primes = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, &primes_sz);

	size = primes_sz;

	srand(time(NULL));
	prime1 = rand() % size;
	prime2 = rand() % size;

	p = primes[prime1];
	q = primes[prime2];
	n = p * q;
	printf("n arxiko = %ld\n", n);
	fi_n = (p - 1) * (q - 1);
	e = choose_e(fi_n);
	printf("e arxiko = %ld\n", e);
	d = mod_inverse(e,fi_n);
	printf("d arxiko =  %ld\n", d);

	public_key[0] = (unsigned char)n;
	public_key[sizeof(unsigned char)] = (unsigned char)e;

	printf("n = %d\n", public_key[0]);
	printf("e =  %d\n", public_key[sizeof(unsigned char)]);

	writeFile("public.key", 2*sizeof(size_t), public_key);

	private_key[0] = (unsigned char)n;
	private_key[sizeof(unsigned char)] = (unsigned char)d;

	printf("n = %d\n", private_key[0]);
	printf("d = %d\n", private_key[sizeof(unsigned char)]);

	writeFile("private.key", 2*sizeof(size_t), private_key);

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

	unsigned long file_length = 0;
	unsigned long key_length = 0;
	unsigned char *input_text;
	unsigned char *output_text;
	unsigned char *input_key;
	size_t n,e;
	int i = 0;

	input_text = NULL;
	output_text = NULL;
	input_key = NULL;

	if(input_file != NULL){
		input_text = readFile(input_file, &file_length);
	} 

	if(key_file != NULL){
		input_key = readFile(key_file, &key_length);
	} 

	output_text = malloc(file_length*sizeof(size_t));

	n = (size_t)(*input_key);
	e = (size_t)(input_key[sizeof(unsigned char)]);

	printf("n = %ld\n", n);
	printf("e = %ld\n", e);

	while(i < file_length){
		output_text[i] = (unsigned char)mod_exponent((size_t)((input_text[i])), 5, 1141);
		i++;
	}

	writeFile(output_file, file_length*sizeof(size_t), output_text);

	free(output_text);
	free(input_text);
	free(input_key);

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

	unsigned long file_length = 0;
	unsigned long key_length = 0;
	unsigned char *input_text;
	unsigned char *output_text;
	unsigned char *input_key;
	size_t n,d;
	int i = 0;

	input_text = NULL;
	output_text = NULL;
	input_key = NULL;

	if(input_file != NULL){
		input_text = readFile(input_file, &file_length);
	} 

	if(key_file != NULL){
		input_key = readFile(key_file, &key_length);
	} 

	output_text = malloc(file_length*sizeof(size_t));

	n = (size_t)(input_key[0]);
	d = (size_t)(input_key[sizeof(unsigned char)]);

	printf("n = %ld\n", n);
	printf("d = %ld\n", d);

	while(i < file_length/8){
		output_text[i] = (unsigned char)mod_exponent((size_t)(input_text[i*8]), 389, 1141);
		i++;
	}

	writeFile(output_file, file_length*sizeof(size_t), output_text);

	free(output_text);
	free(input_text);
	free(input_key);
}