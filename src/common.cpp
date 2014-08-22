#include "common.h"

void COMMON::printHash(const unsigned char *d) {
	for (int i = 0; i < HASH_SIZE; i++)
		printf("%02x", d[i]);
	printf("\n");
}

std_digest COMMON::assignStdigest(digest dg) {
	std_digest ret_dg;
	for (size_t i = 0; i < HASH_SIZE; i++) {
		ret_dg[i] = dg[i];
	}
	return ret_dg;
}
