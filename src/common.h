#ifndef PERMACN_COMMN_H
#define PERMACN_COMMN_H

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <list>
#include <vector>
#include <string>
#include <iterator>
#include <random>
#include <algorithm>
#include <openssl/sha.h>
#include <stdexcept>
#include <iostream>

/// Macros N' typedef structure
#define LEAF_SIZE 1024
#define HASH_SIZE 20
#define KEY_SIZE 256
#define XTRACT_SIZE 10
#define SUBSET_CONST 5
#define ALL_CONST 16
#define CHALNG_CONST 2
#define SEED_LENGTH 20
#define LAMBDA 16

#define HEAD 0
#define FINE 0
#define SAME 0
#define INVALID_ERR -1
#define PTNULL_ERR -2
#define MALLOC_ERR -3
#define FILE_ERR -4

typedef unsigned char uchar;
typedef uchar leaf[LEAF_SIZE];
typedef uchar digest[HASH_SIZE];

namespace COMMON {

/**
 * Print the unchar array in Hex form
 * INPUT unchar array and its length
 */
void printHex(const uchar* ucstr, const size_t length);

/**
 * String in Hex form
 * REF http://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
 * !!!! Not fully tested
 */
std::string stringToHex(const std::string& input);

/**
 * Return true if the value is even
 * INPUT a size_t value; OUTPUT true if it's even
 * !!!! An inline method
 */
template<typename T>
inline bool isEven(T v) {
	return ((v % 2) == 0 ? true : false);
}

/**
 * A random oracle lies within [0,max-1]
 * INPUT hashvalue as a seed
 * OUTPUT a random number ranged [0,max-1]
 */
size_t randomNum(digest hashvalue, size_t max);

/**
 * Generate a k-BYTES random std::string
 * INPUT k (bytes); OUTPUT a random string whose length is k
 */
std::string newRandStr(size_t k);

/**
 * Computes u[i] := H_0(pk||i_inl) mod num_all
 */
size_t computeU_i(std::string pk, size_t i_inl, size_t num_all);

/**
 * Computes H(puz||pk||i_ink||s) mod num_sub
 */
size_t computeI_inL(std::string puz, std::string pk, int i_ink, std::string seed,
		size_t num_sub);

/**
 * Computes r[i] := computeStoreU_i(computeChlngU_i())
 */
size_t computeR_i(std::string puz, std::string pk, int i_ink, std::string seed,
		size_t num_sub, size_t num_all);

}	/// namespace COMMON

#endif
