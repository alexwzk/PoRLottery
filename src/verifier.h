#ifndef PERMACN_VERIFIER_H
#define PERMACN_VERIFIER_H

#include "merkle.h"

#include <fstream>

class VERIFIER {
private:
	std::string puzzle_id;
	uchar* root_digest;
	TICKET* tic_verify;
	size_t num_subset = SUBSET_CONST, num_all = ALL_CONST;

public:

	/**
	 * a default constructor
	 * Set constant values
	 * Allocate memory for the private pointers
	 */
	VERIFIER() {
		root_digest = new digest;
		tic_verify = new TICKET;
	}

	/**
	 * 1st constructor
	 * GOTO resetVERIFIER
	 */
	VERIFIER(std::string root_file, std::string tic_file);

	/**
	 * Destructor: delete root and tic_verify
	 */
	~VERIFIER();

	/**
	 * INTPUT a root digest of the Merkle tree,
	 * 	      a file path from which the ticket info is read
	 * OUTPUT fine
	 */
	int resetVERIFIER(std::string root_file, std::string tic_file);

	/**
	 * Get the puzzle id
	 * OUTPUT fine
	 */
	int getPuzzleID(std::string puz_id);

	//TODO return Puzzle ID

	/**
	 * Verify all challenged segments stored in tic_verify->mkproofs
	 * OUTPUT true if all matched otherwise false
	 */
	bool verifyAllChallenges();
};

#endif
