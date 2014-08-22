#ifndef PERMACN_COMMN_H
#define PERMACN_COMMN_H

#include "zkarray.h"

#include <cstdio>

/// Macro N' typedef structure
#define LEAF_SIZE 1024
#define HASH_SIZE 20
#define KEY_SIZE 256

typedef unsigned char leaf[LEAF_SIZE];
typedef unsigned char digest[HASH_SIZE];
typedef ZK::ARRAY<unsigned char, HASH_SIZE> zk_digest;
typedef ZK::ARRAY<unsigned char, LEAF_SIZE> zk_leaf;

namespace COMMON {

/*
 * printHex - to print the unchar array in Hex form
 * Input - unchar array; Outuput - void; Affect - None
 */
void printHash(const unsigned char *d);

/*
 * template isEven - return true if the value is even
 * Input - a size_t value; output - true or false
 * Affect - none
 */
template<typename T>
inline bool isEven(T v) {
	return ((v % 2) == 0 ? true : false);
}

}	/// namespace COMMON

#endif
