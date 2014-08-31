#ifndef PERMACN_PATH_H
#define PERMACN_PATH_H

#include "common.h"

class PATH {
private:
	uchar* leafPt = nullptr;	// the head pt of item
	std::list<uchar*> siblings; // the item's hash siblings

public:

	/**
	 * 1st constructor: initialised by a leaf
	 */
	PATH(leaf inleaf);

	/**
	 * Destructor: takes care of the memory collection
	 */
	~PATH();

	/**
	 * Return the pointer to its leaf item
	 * Output: leafPt
	 */
	uchar* returnLeafPt();

	/**
	 * Return the list of hash siblings pointers
	 * Output: siblings
	 */
	std::list<uchar*> returnSiblings();

	/**
	 * Pushback the hash sibling into the siblings
	 * Input: a head pointer of the digest
	 * Output: FINE
	 */
	int pushDigestPt(digest hash);

	//TODO: delete or insert digests, rememb to delete memory
};

#define PUZID 0
#define ALLPROFS 1
#define PUBKEY 2
#define SEED 3
#define TICPROFS 4

struct TICKET {
	std::string pubkey;
	std::string seed;
	std::vector<PATH*> mkproofs; //uses allmkproofs' pointers in USER class
};

#endif
