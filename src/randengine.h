/*
 * randengine.h
 *
 *  Created on: Sep 14, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef BTC_RANDENGINE_H_
#define BTC_RANDENGINE_H_

#include <random>
#include <iostream>
#include <openssl/sha.h>
#include <uint256.h>

#define XTRACT_SIZE 10

template<class HASH_TYPE>
class RANDENGINE {
private:

	std::random_device randevice;
	std::default_random_engine rand_engine;

public:
	/**
	 * Turns on the default random engine
	 */
	RANDENGINE() {
		rand_engine.seed(randevice());
	}

	/**
	 * A random oracle lies within [0,max-1]
	 * INPUT hashvalue as a seed
	 * OUTPUT a random number ranged [0,max-1]
	 */
	size_t randByHash(const HASH_TYPE& hashvalue, const size_t max){
		//TODO change it into reject sampling
		//naive approach 2
		size_t from_hash;
		memcpy((uint8_t*) &from_hash, hashvalue.begin(), sizeof(size_t));
		rand_engine.seed(from_hash);
		// coutest hash to size_t
		//std::cout << "to a long int: " << (size_t) (*hashvalue) << std::endl;
		std::uniform_int_distribution<> dis(0, max - 1);
		return dis(rand_engine);
	}

	/**
	 * Generate a k-BYTES random std::string
	 * INPUT k (bytes);
	 * OUTPUT a random string whose length is k
	 */
	std::string newRandStr(const size_t k){
		HASH_TYPE hashvalue;
		size_t remain_k = k, nonce = -1, hash_size = hashvalue.size();
		std::string randstr;
		while (remain_k > hash_size) {
			nonce = rand_engine();
			SHA1((uint8_t *) (&nonce), sizeof(size_t), hashvalue.begin());
			randstr.append(hashvalue.begin(), hashvalue.end());
			remain_k -= hash_size;
		}
		nonce = rand_engine();
		SHA1((uint8_t *) (&nonce), sizeof(size_t), hashvalue.begin());
		randstr.append(hashvalue.begin(), hashvalue.end());
		return randstr;
	}

	/**
	 *TODO A vector of random number sequence
	 *All elements lie within [0, max-1]
	 *INPUT size of the vector, maximum number
	 *OUTPUT the random numbers sequence
	 */
	/*std::vector<size_t> randSequence(size_t size, size_t max) {

	}*/

};

#endif /* BTC_RANDENGINE_H_ */
