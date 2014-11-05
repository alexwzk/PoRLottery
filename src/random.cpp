#include "random.h"

size_t PMC::getRandByHash(const uint160& hashvalue, const size_t max){
	//TODO Rejecting sampling
	return hashvalue.GetLow64() % max;
}

uint160 PMC::getRandHash(){
    uint160 hash;
    GetRandBytes((unsigned char*)&hash, sizeof(hash));
    return hash;
}
