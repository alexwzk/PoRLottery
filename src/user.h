#ifndef PERMACN_USER_H
#define PERMACN_USER_H

#include "common.h"
#include "merkle.h"

class USER {
private:
	std::string mypubkey;
	std::string seed;
	std::set<PATH> mkproofs;
	size_t num_subset = SUBSET_CONST;
	int chalng_times = CHALNG_CONST;

	/**
	 * Generates a random string seed
	 * Input : Null; Output: Void
	 * Affect: this.seed
	 */
	void newSeed();

	/**
	 * TODO: new challenge numbers
	 */

public:
	/**
	 * 1st constructor which must be initiated by a public key
	 */
	USER(std::string pubkey);

	/**
	 * Receive the paths set from the dealer
	 * Input: set of the item and its merkle proof;
	 * Output: the new size of mkproofs
	 * Affect: this.mkproofs
	 */
	int getMKProofs(std::set<PATH> inmkproofs);

	/**
	 * TODO: generate a ticket, the ticket self-struct
	 */

};

#endif
