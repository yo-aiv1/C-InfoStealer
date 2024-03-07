#include <stdio.h>

#include <../include/copy.h>


int copy(const char *SrcPath, const char *DestPath) {
	FILE* SrcFile = fopen(SrcPath, "rb");
	FILE* DestFile = fopen(DestPath, "wb");

	if (SrcFile == NULL || DestFile == NULL) {
		return 1;
	}

	char buffer[4096];
	size_t BytesRead;

	while ((BytesRead = fread(buffer, 1, sizeof(buffer), SrcFile)) > 0) {
		fwrite(buffer, 1, BytesRead, DestFile);
	}

	fclose(SrcFile);
	fclose(DestFile);

	return 0;
}