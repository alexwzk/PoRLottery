#ifndef PERMACN_USER_H
#define PERMACN_USER_H

#include "path.h"

#include <bitset>
#include <fstream>

class USER {
private:
	size_t num_subset = SUBSET_CONST;
	int i_ink = CHALNG_CONST;
	std::bitset<5> flags;
	std::string puzzle_id;
	std::vector<PATH*> allmkproofPts;
	TICKET* myticketPt;
	RANDENGINE* rand_enginePt;

	/**
	 * 1st Constructor: generates the ticket
	 * OUTPUT FINE
	 * AFFECT myticket.mkproofs
	 */
	int generateTicket();

	/**
	 * Is ready to release the ticket?
	 */
	bool isReadyToRelease();

public:

	/**
	 * 1st constructor: starts with the user's public key
	 */
	USER(std::string pubkey);

	/**
	 * Destructor: deletes memory in allmkproofs, ticket
	 */
	~USER();

	/**
	 * Read paths from file
	 * INPUT file path
	 * OUTPUT FINE or FILE_ERR or MALLOC_ERR
	 * AFFECT allmkproofs & myticket->mkproofs
	 */
	int storeFile(std::string inputf);

	/**
	 * Receives a new puzzle ID
	 * INTPUT a Puzzle id
	 * OUTPUT FINE
	 * AFFECT this->puzzle_id
	 */
	int getNewPuzzle(std::string id);

	/**
	 * Generates a new random seed
	 */
	void resetSeed();

	/**
	 * Resets the user's public key
	 * INPUT a new public key
	 * AFFECT this->myticket.pubkey
	 */
	void resetPubkey(std::string newpbkey);

	/**
	 * Reveals a new copy of the user's public key
	 * OUTPUT this->myticket.pubkey
	 */
	std::string returnMyPubkey();

	/**
	 * Writes the ticket to file
	 * INPUT output file path
	 * OUTPUT FINE or FILE_ERR
	 */
	int writeTicket(std::string outf);

};

#endif
