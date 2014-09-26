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

size_t RANDENGINE::randByHash(digest hashvalue, size_t max) {
	rand_engine.seed((size_t)(*hashvalue)); //Does this affect the newRandStr? Is this way safe?
	//Coutest
	std::cout << "Cast the first 4 bytes into a long int: " << (size_t)(*hashvalue) << std::endl;
	std::uniform_int_distribution<> dis(0, max-1);
	return dis(rand_engine);
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
