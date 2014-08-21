//TODO - add copyright and software license

#ifndef PERMACN_MERKLE_H //for avoid multi-inclusions of header files
#define PERMACN_MERKLE_H
#include "common.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <stdexcept>
#include <iostream>
#include <list>
#include <iterator>
#include <openssl/sha.h>

struct path {
	leaf item;
	std::list<std_digest> siblings;
};
// to validate the integrity of an item

class MERKLE {
private:
	size_t num_segmts, num_leaves, height;
	leaf *segments;
	digest **arrays;

	int now_layer, next_layer;

public:

	/*
	 * Merkle tree 1st constructor
	 * Input - the segments of entire file (segment is in fixed length) & their size;
	 * Ouput - void; Affect - initialise the private data source
	 */
	MERKLE(leaf *segments, size_t num_segs);

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
