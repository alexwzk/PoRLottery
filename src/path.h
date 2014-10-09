#ifndef PERMACN_PATH_H
#define PERMACN_PATH_H

#include "pmc.h"

class PATH {
private:
	uint8_t* leafPt;	// the head pt of item
	size_t leaf_size;
	std::list<uint8_t*> siblings; // the item's hash siblings

public:

	/**
	 * 1st constructor: initialised by a leaf
	 */
	PATH(uint8_t* inleaf, size_t leaf_size);

	/**
	 * Destructor: deletes leafPt and pointers stored in siblings
	 */
	~PATH();

	/**
	 * Releases the pointer to its leaf item
	 * INPUT get the leaf's size
	 * OUTPUT leafPt
	 */
	uint8_t* releaseLeaf(size_t &leaf_size);

	/**
	 * Returns the list of hash siblings pointers
	 * OUTPUT siblings
	 */
	std::list<uint8_t*> returnSiblings();

	/**
	 * Push backs the hash sibling into the siblings
	 * INPUT a head pointer of the digest
	 * OUTPUT FINE or NULLPT_ERR if hash is invalid
	 */
	int pushDigestPt(digest hash);

	/**
	 * OUTPUT a new copy of string consisting of the leaf and Merkle proofs
	 */
	std::string returnPathAsStr();

	//TODO delete or insert digests, remember to delete memory before deleting digests
};
#endif
