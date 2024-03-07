#ifndef _DECRYPTER_
#define _DECRYPTER_

#define IV_START_INDEX 3
#define IV_END_INDEX 15

unsigned char *DataDecryptor(unsigned char *value, int size, unsigned char *key);

#endif