#ifndef PERMACN_MERKLE_H //to avoid multi-inclusions of header files
#define PERMACN_MERKLE_H

#include "path.h"

class MERKLE {
private:
	size_t num_segmts = 0, num_leaves = 0, height = 0;
	leaf *segments = nullptr;	//a buffer-like array
	digest **arrays = nullptr;//Should be treated as a hierarchical chunks of buffer

	int now_layer = -1, next_layer = -1;

public:

	/**
	 * Merkle tree 1st constructor
	 * Input: a vector of pointers to every file segment
	 * Affect: initialise all private data
	 */
	MERKLE(std::vector<uchar *> segms);

	/**
	 * Merkle tree destructor
	 * Affect: release dynamic memory allocation
	 */
	~MERKLE();

	/**
	 * Builds the Merkle proof for the corresponding segment
	 * Input: the location of target segment;
	 * Output: a self-defined data struct (path) including
	 * the segment and its Merkle proof (an array of pointers to each sibling digest)
	 */
	PATH* newPath(size_t loca);

	/**
	 * Release the root digest
	 * Output: a uchar array pointer to the digest
	 * 		   or nullptr if not ready
	 */
	uchar* releaseRootPt();

	//TODO: copy & assignment

};

#endif
