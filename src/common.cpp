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

std::string newRandStr(size_t k){
	std::default_random_engine rand_engine;
	size_t nonce = rand_engine();
	std::string randstr;
	unsigned char hashv[HASH_SIZE];
	SHA1(reinterpret_cast<const unsigned char*>(&nonce),sizeof(size_t),hashv);
	//TODO: while loop until reaches k bits
	return randstr;
}
