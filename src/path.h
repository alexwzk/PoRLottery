#ifndef PERMACN_PATH_H
#define PERMACN_PATH_H

#include "common.h"

class PATH {
private:
	uchar* leafPt;	// the head pt of item
	std::list<uchar*> siblings; // the item's hash siblings

public:

	/**
	 * 1st constructor
	 * Initialised by a leaf
	 */
	PATH(leaf inleaf);

	/**
	 * Destructor
	 * Delete leafPt and pointers stored in siblings
	 */
	~PATH();

	/**
	 * Release the pointer to its leaf item
	 * OUTPUT leafPt
	 */
	uchar* releaseLeafPt();

	/**
	 * Return the list of hash siblings pointers
	 * OUTPUT siblings
	 */
	std::list<uchar*> returnSiblings();

	/**
	 * Pushback the hash sibling into the siblings
	 * INPUT a head pointer of the digest
	 * OUTPUT FINE or NULLPT_ERR if hash is invalid
	 */
	int pushDigestPt(digest hash);

	//TODO: delete or insert digests, remember to delete memory before deleting digests
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
