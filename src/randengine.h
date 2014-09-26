/*
 * randengine.h
 *
 *  Created on: Sep 14, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef RANDENGINE_H_
#define RANDENGINE_H_

#include <random>
#include <iostream>
#include <openssl/sha.h>

#define XTRACT_SIZE 10
#define HASH_SIZE 20

typedef unsigned char uchar;
typedef uchar digest[HASH_SIZE];

class RANDENGINE {
private:
	std::random_device randevice;
	std::default_random_engine rand_engine;

public:
	/**
	 * Turns on the default random engine
	 */
	RANDENGINE();

	/**
	 * Null destructor
	 */
	~RANDENGINE(){}

	/**
	 * A random oracle lies within [0,max-1]
	 * INPUT hashvalue as a seed
	 * OUTPUT a random number ranged [0,max-1]
	 */
	size_t randByHash(digest hashvalue, size_t max);

	/**
	 * Generate a k-BYTES random std::string
	 * INPUT k (bytes); OUTPUT a random string whose length is k
	 */
	std::string newRandStr(size_t k);


};

#endif /* RANDENGINE_H_ */
