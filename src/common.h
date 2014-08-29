#ifndef PERMACN_COMMN_H
#define PERMACN_COMMN_H

#include "zkarray.h"

#include <openssl/sha.h>
#include <cstdio>
#include <random>
#include <algorithm>
#include <string>
#include <stdexcept>

/// Macros N' typedef structure
#define LEAF_SIZE 1024
#define HASH_SIZE 20
#define KEY_SIZE 256
#define XTRACT_SIZE 10
#define SUBSET_CONST 2
#define ALL_CONST 4
#define CHALNG_CONST 1
#define SEED_LENGTH 160

#define HEAD 0
#define SUCCESS 0
#define FILE_ERR 1
#define INVALID_ERR -1

typedef unsigned char leaf[LEAF_SIZE];
typedef unsigned char digest[HASH_SIZE];
typedef ZK::ARRAY<unsigned char, HASH_SIZE> zk_digest;
typedef ZK::ARRAY<unsigned char, LEAF_SIZE> zk_leaf;

namespace COMMON {

/**
 * printHex - to print the unchar array in Hex form
 * Input - unchar array; Outuput - void; Affect - None
 */
void printHash(const unsigned char *d);

/**
 * String in Hex form
 * http://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
 * Not tested
 */
std::string stringToHex(const std::string& input);

/**
 * template isEven - return true if the value is even
 * Input - a size_t value; output - true or false
 * Affect - none
 */
template<typename T>
inline bool isEven(T v) {
	return ((v % 2) == 0 ? true : false);
}

/**
 * extract a small number from the hashvalue
 *
 */
size_t hashToNumber(digest hashv);

/**
 * generate a k-bits random std::string
 * Input - unsigned int k; output - std::string
 * Affect - none
 */
std::string newRandStr(size_t k);

}	/// namespace COMMON

#endif
