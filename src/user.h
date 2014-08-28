#ifndef PERMACN_USER_H
#define PERMACN_USER_H

#include "common.h"
#include "merkle.h"

struct TICKET {
	std::string pubkey;
	std::string seed;
	std::vector<PATH> mkproofs;
};

class USER {
private:
	size_t num_subset = SUBSET_CONST;
	int chalng_times = CHALNG_CONST;
	bool ready_to_release = false;
	std::vector<size_t> chalng_pos;
	std::vector<PATH> allmkproofs;
	TICKET myticket;

	/**
	 * Generates a random string seed
	 * Input : Null; Output: Void
	 * Affect: this->myticket.seed
	 */
	void newSeed();

	/**
	 * Generate a vector of subscripts (a list of random numbers within SUBSET_CONST)
	 * then sort them
	 * Input: Nulll; Output: Void
	 * Affect: this->chalng_pos
	 */
	void newChallenges();

	/**
	 * Reset user's public key
	 * Input: a new public key; Output: Void
	 * Affect: this->myticket.pubkey
	 */
	void resetPubkey(std::string newpbkey);

public:
	/**
	 * 1st constructor which must be initiated by a public key
	 */
	USER(std::string pubkey);

	/**
	 * Receive the paths set from the dealer
	 * Input: set of the item and its merkle proof;
	 * Output: the new size of mkproofs
	 * Affect: this->allmkproofs
	 */
	int getMKProofs(std::vector<PATH> inmkproofs);

	/**
	 * Reveal my public key
	 * Input: Null; Output: this->myticket.pubkey
	 * Affect: None
	 */
	std::string returnMyPubkey();

	/**
	 * Release the ticket to verifiers
	 * Input: Null; Output: this->myticket
	 * Affect: None
	 */
	TICKET releaseTicket();

};

#endif
