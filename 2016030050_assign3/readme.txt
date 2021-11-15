Encryption looks like it works but when i decrypt files i got problems .
While debugging i found that inputs outputs worked alright.So everything i write or read from a file was correct.
However the decryption was not done correctly due to modulo exponent function (or 
size_t encrypt(size_t message, size_t exponent_d, size_t n) function in my code).
Also Keygen looks like it works fine.
