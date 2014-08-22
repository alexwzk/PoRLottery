#ifndef PERMACN_COMMN_H
#define PERMACN_COMMN_H

#include <cstdio>
#include <algorithm>
#include <array>

/// Macro N' typedef structure
#define LEAF_SIZE 1024
#define HASH_SIZE 20
#define KEY_SIZE 256

typedef unsigned char leaf[LEAF_SIZE];
typedef unsigned char digest[HASH_SIZE];
typedef std::array<unsigned char, HASH_SIZE> std_digest;

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
template<class C>
inline bool isEven(C v) {
	return ((v % 2) == 0 ? true : false);
}

/*
 * Assign a std::digest
 * Input - c-srtring digest pointer
 * Return - a std_digest
 */
std_digest assignStdigest(digest dg);

}	/// namespace COMMON

#endif
