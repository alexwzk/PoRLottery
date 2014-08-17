#ifndef PERMACN_COMMN_H
#define PERMACN_COMMN_H

#include <cstdio>
#include <cstdlib>

// Internal implementation code
namespace {

namespace COMMON {

/*
 * printHex - to print the unchar array in Hex form
 * Input - unchar array; Outuput - void; Affect - None
 */
void printHex(const unsigned char *d) {
	for (int i = 0; i < 20; i++)
		printf("%02x", d[i]);
	printf("\n");
}

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

}	/// namespace

#endif
