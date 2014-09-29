#ifndef PERMACN_PATH_H
#define PERMACN_PATH_H

#include "common.h"

class PATH {
private:
	uchar* leafPt;	// the head pt of item
	std::list<uchar*> siblings; // the item's hash siblings

public:

	/**
	 * 1st constructor: initialised by a leaf
	 */
	PATH(leaf inleaf);

	/**
	 * Destructor: deletes leafPt and pointers stored in siblings
	 */
	~PATH();

	/**
	 * Releases the pointer to its leaf item
	 * OUTPUT leafPt
	 */
	uchar* releaseLeafPt();

	/**
	 * Returns the list of hash siblings pointers
	 * OUTPUT siblings
	 */
	std::list<uchar*> returnSiblings();

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
