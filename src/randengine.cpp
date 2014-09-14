/*
 * randengine.cpp
 *
 *  Created on: Sep 14, 2014
 *      Author: Zikai Alex Wen
 */

#include "randengine.h"

RANDENGINE::RANDENGINE() {
	rand_engine.seed(randevice());
}

RANDENGINE::~RANDENGINE() {
}

size_t RANDENGINE::randFromHash(digest hashvalue, size_t max) {
	// TODO: Uniform distribution: leftshift 256( as to accumulate bytes )
	// bit at a time: 2
	size_t number = 0;
	for (int i = 0; i < XTRACT_SIZE; i++) {
		number += hashvalue[i];
	}
	return number % max;
}

std::string RANDENGINE::newRandStr(size_t k) {
	size_t remain_k = k;
	size_t nonce = -1;
	std::string randstr;
	digest hashvalue;
	while (remain_k > HASH_SIZE) {
		nonce = rand_engine();
		SHA1((uchar *) (&nonce), sizeof(size_t), hashvalue);
		randstr.append(hashvalue, hashvalue + HASH_SIZE);
		remain_k -= HASH_SIZE;
	}
	nonce = rand_engine();
	SHA1((uchar *) (&nonce), sizeof(size_t), hashvalue);
	randstr.append(hashvalue, hashvalue + remain_k);
	return randstr;
}
