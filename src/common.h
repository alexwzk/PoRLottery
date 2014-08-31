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
#define SUBSET_CONST 2
#define ALL_CONST 4
#define CHALNG_CONST 1
#define SEED_LENGTH 160	//bits

#define NO_CHANGE 2
#define FILE_ERR 1
#define HEAD 0
#define FINE 0
#define INVALID_ERR -1
#define PTNULL_ERR -2

typedef unsigned char uchar;
typedef uchar leaf[LEAF_SIZE];
typedef uchar digest[HASH_SIZE];

namespace COMMON {

/**
 * printHex - to print the unchar array in Hex form
 * Input - unchar array; Outuput - void; Affect - None
 */
void printHex(const uchar* ucstr, const size_t length);

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
