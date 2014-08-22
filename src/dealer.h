#ifndef PERMACN_DEALER_H
#define PERMACN_DEALER_H

#include "common.h"
#include "merkle.h"

#include <cstdlib>

class DEALER {
private:
	char public_key[KEY_SIZE];
	bool is_ready;
	MERKLE merkle_tree;
	size_t *subset_segPt;
	size_t num_subset, num_all;
	path *path_list;

	/*
	 * createSubset - generates the S_{pk}
	 * Input - nothing; Output - 0 is success, otherwise -1 means is_ready == false;
	 * Affect - subset_segPt
	 */
	int createSubset();

public:
	/*
	 * DEALER 1st constructor - computes the Merkle tree of the entire dataset
	 * Input - the file path; Output - void; Affect - initialise the merkle_tree
	 * ! The file should include the total number of data segments
	 */
	DEALER(char *filenamePt);

//	TODO - destructor, copy and assignment functions

	/*
	 *	createSource - setup the permacoin for a participant
	 *	Input - an array of char which is the public key;
	 *	Output - a set of file segments and their Merkle proofs (path list);
	 *	Affect - all private data except the merkle_tree;
	 */
	path* createSource(char *user_pkPt);

};

#endif
