gcc -Wextra -Wall -Werror -o dll/main.exe src/copy.c src/_strcat.c src/main.c src/vaultkey.c src/browser.c src/decryptor.c -Iinclude -Llib -lssl -lcjson -lcrypto -lcrypt32  -lsqlite3
