#ifndef PERMACN_MERKLE_H //to avoid multi-inclusions of header files
#define PERMACN_MERKLE_H

#include "common.h"

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <stdexcept>
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <openssl/sha.h>

class PATH {
public:
	zk_leaf item;
	std::list<zk_digest> siblings;
	friend bool operator==(const PATH& p1, const PATH& p2) {
		return p1.item == p2.item;
	}
	friend bool operator<(const PATH& p1, const PATH& p2) {
		return p1.item < p2.item;
	}
	friend bool operator>(const PATH& p1, const PATH& p2) {
		return p1.item > p2.item;
	}
};

class MERKLE {
private:
	size_t num_segmts = 0, num_leaves = 0, height = 0;
	leaf *segments = nullptr;	//a buffer-like array
	digest **arrays = nullptr;//Should be treated as a hierarchical chunks of buffer

	int now_layer = -1, next_layer = -1;

public:

	/**
	 * Merkle tree 1st constructor
	 * Input - a vector of file segments
	 * Ouput - void; Affect - initialise the private data source
	 */
	MERKLE(std::vector<zk_leaf> segms);

	/**
	 * buildPath - builds the Merkle proof for the corresponding segment
	 * Input - the location of wanted segment;
	 * Output - a self-defined data struct (path) including
	 * the segment and its Merkle proof (an array of sibling digests)
	 * Affect - none
	 */
	PATH buildPath(size_t loca);

	/**
	 * Merkle tree destructor
	 * Input - null; Output - void; Affect - release dynamic memory allocation
	 */
	~MERKLE();

	//TODO - Need copy & assignment

	/**
	 * returnHeight - returns the height of this Merkle tree
	 * Input - null; Output - the private data: height; Affect - none
	 */
	int returnHeight();
};

#endif
