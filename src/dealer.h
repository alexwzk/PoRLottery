#ifndef PERMACN_DEALER_H
#define PERMACN_DEALER_H

#include "common.h"
#include "merkle.h"

#include <set>

class DEALER {
private:

	MERKLE *mktreePt = nullptr;
	char pubkey[KEY_SIZE];
	size_t num_subset = 5, num_all = 8; //TODO: set as a macro?
	std::vector<size_t> subset_pk;
	std::set<path> path_set;
	bool is_ready = false;

	/*
	 * createSubset - generates the S_{pk}
	 * Input - nothing; Output - 0 is success, otherwise -1 means not ready yet (no new key);
	 * Affect - updates subset_pk, path_set and set is_ready as false
	 */
	int createSubset();

public:
	/*
	 * DEALER 1st constructor - computes the Merkle tree of the entire dataset
	 * Input - the file path; Output - void; Affect - initialise the merkle_tree
	 */
	DEALER(char *filenamePt);

//	TODO - destructor, copy and assignment functions

	/*
	 *	createSource - setup the permacoin for a participant
	 *	Input - an array of char which is the public key;
	 *	Output - a set of file segments and their Merkle proofs (path list);
	 *	Affect - all private data except the merkle_tree;
	 */
	std::set<path> createSource(char *user_pkPt);

};

#endif
