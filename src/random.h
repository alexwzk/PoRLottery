/*
 * randengine.h
 *
 *  Created on: Sep 14, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_RANDOM_H_
#define PMC_RANDOM_H_

#include <uint256.h>	//BTC
#include <hash.h>		//BTC
#include <random.h>		//BTC

namespace PMC {
	/**
	 * A random oracle lies within [0,max]
	 * INPUT hashvalue as a seed
	 * OUTPUT a random number ranged [0,max]
	 */
	size_t getRandByHash(const uint256& hashvalue, const size_t max);

};

#endif /* PMC_RANDOM_H_ */
