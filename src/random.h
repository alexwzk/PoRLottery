/*
 * randengine.h
 *
 *  Created on: Sep 14, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_RANDOM_H_
#define PMC_RANDOM_H_

#include <openssl/sha.h>
#include <uint256.h>	//BTC
#include <hash.h>		//BTC
#include <random.h>		//BTC

namespace PMC {
	/**
	 * A random oracle lies within [0,max-1]
	 * INPUT hashvalue as a seed
	 * OUTPUT a random number ranged [0,max-1]
	 */
	size_t getRandByHash(const uint160& hashvalue, const size_t max);

	uint160 getRandHash();

};

#endif /* PMC_RANDOM_H_ */
