#ifndef PERMACOIN_H_
#define PERMACOIN_H_

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <version.h> // CLIENT_VERSION

#include "randengine.h"

namespace pmc {

/// Macros & typedef structure
#define ALL_CONST 16
#define SUBSET_CONST 5
#define CHALNG_CONST 2
#define FPS_LEAFSIZE 16
#define SEED_LENGTH 20

#define HEAD 0
#define FINE 0
#define SAME 0
#define INVALID_ERR -1
#define PTNULL_ERR -2
#define MALLOC_ERR -3
#define FILE_ERR -4

/**
 * Convert an array of unsigned char to the vector form
 * INPUT pointer to the built-in array, the length of the array
 * OUTPUT the vector form of an array
 */
std::vector<uint8_t>* newByteVec(uint8_t* a_pointer, size_t a_length);

/**
 * Prints the unchar array in Hex form
 * INPUT unchar array and its length
 */
void printHex(const uint8_t* ucstr, const size_t length);

/**
 * Returns string in Hex form
 * REF http://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
 * !!!! Not fully tested
 */
std::string stringToHex(const std::string& input);

/**
 * Returns true if the value is even
 * INPUT a size_t value; OUTPUT true if it's even
 * !!!! An inline method
 */
template<typename T>
inline bool isEven(T v) {
	return ((v % 2) == 0 ? true : false);
}

/**
 * Computes u[i] := H_0(pk||i_inl) mod num_all
 */
template<class HASH_TYPE>
size_t computeU_i(HASH_TYPE pk, size_t i_inl, size_t num_all) {
	HASH_TYPE hashvalue;
	RANDENGINE<HASH_TYPE> rand_engine;
	std::string tmp_str = pk.ToString() + std::to_string(i_inl);

	// coutest pk||i
	SHA1((uint8_t *) tmp_str.data(), tmp_str.size(), hashvalue.begin());
	// coutest state
	return rand_engine.randByHash(hashvalue, num_all);
}

/**
 * Computes H(inputs) mod num_sub
 */
template<class HASH_TYPE>
size_t computeI_inL(std::string inputs, size_t num_sub) {
	HASH_TYPE hashvalue;
	RANDENGINE<HASH_TYPE> rand_engine;

	SHA1((uint8_t *) inputs.data(), inputs.size(), hashvalue.begin());
	return rand_engine.randByHash(hashvalue, num_sub);
}

/**
 * Computes r[i] := computeStoreU_i(computeChlngU_i())
 */
template<class HASH_TYPE>
size_t computeR_i(HASH_TYPE pk, std::string inputs, size_t num_sub,
		size_t num_all){
	return computeU_i(pk, computeI_inL<HASH_TYPE>(inputs, num_sub), num_all);
}

}	/// namespace PMC (shorten for Permacoin)

#endif /*PERMACOIN_H_*/
