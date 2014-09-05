#ifndef PERMACN_USER_H
#define PERMACN_USER_H

#include "path.h"

#include <bitset>
#include <fstream>

class USER {
private:
	size_t num_subset = SUBSET_CONST;
	int i_ink = CHALNG_CONST;
	std::string puzzle_id;
	std::vector<PATH*> allmkproofs;
	TICKET* myticket = nullptr;
	std::bitset<5> flags;

	/**
	 * Generate the ticket, will call the private sub-method: newChallenges()
	 * input: Null; Output: 0 success
	 * Affect: myticket.mkproofs
	 */
	int generateTicket();

	/**
	 * Check is ready to release the ticket
	 */
	bool isReadyToRelease();

public:
	/**
	 * 1st constructor which must be initiated by a public key
	 */
	USER(std::string pubkey);

	/**
	 * Destructor
	 * Delete memory in allmkproofs, ticket
	 */
	~USER();

	/**
	 * Read paths from file
	 * Intput: file path
	 * Output: FINE or FILE_ERR or MALLOC_ERR
	 * Affect: allmkproofs & myticket->mkproofs
	 */
	int storeFile(std::string inputf);

	/**
	 * Receive a new puzzle ID
	 * Input: a Puzzle id; Output: -1 if the id wasn't changed
	 * Affect: this->puzzle_id
	 */
	int getNewPuzzle(std::string id);

	/**
	 * Choose new random seed
	 */
	void resetSeed();

	/**
	 * Reset user's public key
	 * Input: a new public key; Output: Void
	 * Affect: this->myticket.pubkey
	 */
	void resetPubkey(std::string newpbkey);

	/**
	 * Reveal my public key
	 * Input: Null; Output: this->myticket.pubkey
	 * Affect: None
	 */
	std::string returnMyPubkey();

	/**
	 * Release the ticket to file
	 * Input: out file path
	 * Output: FINE or FILE_ERR
	 */
	int releaseTicket(std::string outf);

};

#endif
