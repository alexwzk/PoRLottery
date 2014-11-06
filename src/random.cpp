#include "random.h"

size_t PMC::getRandByHash(const uint256& hashvalue, const size_t max){
	//TODO Rejecting sampling
	return hashvalue.GetLow64() % max;
}
