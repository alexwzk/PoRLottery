#include "common.h"

void COMMON::printHash(const unsigned char *d) {
	for (int i = 0; i < HASH_SIZE; i++)
		printf("%02x", d[i]);
	printf("\n");
}

size_t COMMON::hashToNumber(digest hashv){
	size_t number = 0;
	for(int i = 0; i < XTRACT_SIZE; i++){
		number += hashv[i];
	}
	return number;
}
