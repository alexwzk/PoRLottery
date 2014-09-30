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
	//TODO change it into reject sampling
	//naive approach 2
	size_t from_hash;
	memcpy((uchar*) &from_hash, hashvalue, sizeof(size_t));
	rand_engine.seed(from_hash); //Does this affect the newRandStr? Is this way safe?
	// coutest hash to size_t
	//std::cout << "to a long int: " << (size_t) (*hashvalue) << std::endl;
	std::uniform_int_distribution<> dis(0, max - 1);
	return dis(rand_engine);
	/* naive approach 1
	 * size_t from_hash;
	 * memcpy((uchar*)&from_hash,hashvalue+4,sizeof(size_t));
	 * return from_hash % max;*/
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
