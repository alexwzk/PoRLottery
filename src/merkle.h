#ifndef PERMACN_MERKLE_H //to avoid multi-inclusions of header files
#define PERMACN_MERKLE_H

#include "path.h"

class MERKLE {
private:
	size_t num_segmts, num_leaves, height;
	leaf *segments;
	digest **arrays;	//a hierarchical chunks of buffer

	int now_layer, next_layer;

public:

	/**
	 * Merkle tree 1st constructor
	 * INPUT a vector of pointers to every file segment, each segment's size
	 * AFFECT initialise all private data
	 */
	MERKLE(std::vector<uchar *> segmts, size_t leaf_size);

	/**
	 * Merkle tree destructor
	 * AFFECT segments[] and arrays[][] are released
	 */
	~MERKLE();

	/**
	 * Builds the Merkle proof for the corresponding segment
	 * INPUT the subscript of desired leaf
	 * OUTPUT self-defined PATH data including
	 *        the segment and its Merkle proof (an array of pointers to each sibling digest)
	 *        or nullptr if not ready
	 */
	PATH* newPath(size_t loca);

	/**
	 * Release the root digest
	 * OUTPUT a uchar array pointer to the digest
	 * 		  or nullptr if not ready
	 */
	uchar* releaseRootPt();

	/**
	 *OUTPUT num_segmts of the Merkle tree
	 */
	size_t returnNumSegmts();

	/**
	 *Validates the received path is legitimate in a merkle tree
	 *INPUT a pointer to the merkle proof, the leaf's size,
	 *      its challenge index and the root digest
	 *OUTPUT true if it's correct otherwise false
	 *!!!! a static member function
	 */
	static bool validatePath(PATH* p, size_t leaf_size, size_t u_i,
			digest root);

	//TODO: copy & assignment

};

#endif
