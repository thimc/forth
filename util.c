#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

char *read_file(const char *path) {
	FILE *fp = fopen(path, "r");
	if (fp == NULL) return NULL;
	if (fseek(fp, 0, SEEK_END) < 0) goto error;
	size_t file_size = ftell(fp);
	if (fseek(fp, 0, SEEK_SET) < 0) goto error;
	char *file_contents = malloc(sizeof(file_contents) * file_size);
	if (file_contents == NULL) goto error;
	fread(file_contents, sizeof(file_contents), file_size, fp);
	file_contents[file_size] = '\0';
	fclose(fp);
	return file_contents;

error:
	fclose(fp);
	return NULL;
}

int iswhitespace(char c) {
	return c == ' ' || c== '\n' || c == '\t' || c == '\r';
}

int isnumber(char c) {
	return c >= '0' && c <= '9';
}

void to_lower(char *s) {
	for (size_t i = 0; i < strlen(s); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] += ('a' - 'A');
		}
	}
}

char *shift_args(int *argc, char ***argv) {
    assert(*argc > 0);
    char *result = **argv;
    (*argv) += 1;
    (*argc) -= 1;
    return result;
}
