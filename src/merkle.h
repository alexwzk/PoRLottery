#ifndef PERMACN_MERKLE_H //to avoid multi-inclusions of header files
#define PERMACN_MERKLE_H

#include "common.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <stdexcept>
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <openssl/sha.h>

struct path {
	zk_leaf item;
	std::list<zk_digest> siblings;
};
// to validate the integrity of an item

class MERKLE {
private:
	size_t num_segmts, num_leaves, height;
	leaf *segments;		//a buffer-like array
	digest **arrays;	//Should be treated as a hierarchical chunks of buffer

	int now_layer, next_layer;

public:

	/*
	 * Merkle tree 1st constructor
	 * Input - a vector of file segments
	 * Ouput - void; Affect - initialise the private data source
	 */
	MERKLE(std::vector<zk_leaf> segms);

	/*
	 * buildPath - builds the Merkle proof for the corresponding segment
	 * Input - the location of wanted segment;
	 * Output - a self-defined data struct (path) including
	 * the segment and its Merkle proof (an array of sibling digests)
	 * Affect - none
	 */
	path buildPath(size_t loca);

	/*
	 * Merkle tree destructor
	 * Input - null; Output - void; Affect - release dynamic memory allocation
	 */
	~MERKLE();

	//TODO - Need copy & assignment

	/*
	 * returnHeight - returns the height of this Merkle tree
	 * Input - null; Output - the private data: height; Affect - none
	 */
	int returnHeight();
};

#endif
