#include "common.h"

void COMMON::printHash(const unsigned char *d) {
	for (int i = 0; i < HASH_SIZE; i++)
		printf("%02x", d[i]);
	printf("\n");
}
