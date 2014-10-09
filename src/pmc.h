#ifndef PERMACN_H
#define PERMACN_H

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <list>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "randengine.h"

namespace pmc {

/// Macros N' typedef structure
#define LEAF_SIZE 1024
#define KEY_SIZE 256
#define SUBSET_CONST 5
#define ALL_CONST 16
#define CHALNG_CONST 2
#define SEED_LENGTH 20
#define LAMBDA 16
#define HEIGHT_FPS 8

#define HEAD 0
#define FINE 0
#define SAME 0
#define INVALID_ERR -1
#define PTNULL_ERR -2
#define MALLOC_ERR -3
#define FILE_ERR -4

typedef uint8_t leaf[LEAF_SIZE];

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
size_t computeU_i(digest pk, size_t i_inl, size_t num_all);

/**
 * Computes H(inputs) mod num_sub
 */
size_t computeI_inL(std::string inputs, size_t num_sub);

/**
 * Computes r[i] := computeStoreU_i(computeChlngU_i())
 */
size_t computeR_i(digest pk, std::string inputs, size_t num_sub,
		size_t num_all);

}	/// namespace PMC (shorten for Permacoin)

#endif
