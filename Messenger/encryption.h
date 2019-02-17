//
//encryption.h
//Messenger
//

void encryption(void);
int len(const char *a);
int intlen(const int *d);
int* conv_key(const char* key);
char* crypter(const char* text, const char* key);
char* decrypter(const char* text, const char* decryptKey);
