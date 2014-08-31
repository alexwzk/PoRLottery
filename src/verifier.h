#ifndef PERMACN_VERIFIER_H
#define PERMACN_VERIFIER_H

#include "path.h"

#include <fstream>

class VERIFIER {
private:
	std::string puzzle_id;
	size_t num_subset = SUBSET_CONST, num_total = ALL_CONST;
	uchar* root_digest = nullptr;
	TICKET* tic_verify = nullptr;

public:

	/**
	 * 1st constructor: allocate memory for the private pointers
	 */
	VERIFIER() {
		root_digest = new digest;
		tic_verify = new TICKET;
	}

	/**
	 * 2nd constructor: a root digest of the Merkle tree,
	 * a file path from which the ticket info is read
	 */
	VERIFIER(digest root, std::string tic_file);

	/**
	 * Destructor: delete root and tic_verify
	 */
	~VERIFIER();

	/**
	 * Computes the elements of u as necessary
	 * Input: the challenge index of the subset
	 * Output: the challenge index of entire database
	 */
	size_t computesUElem(int k_i);

	/**
	 *Validates the received path is legitimate in this merkle tree
	 *Input: a pointer to the PATH data and its challenge index
	 *Output: true if it's correct otherwise false
	 */
	bool validatePath(PATH* p, size_t u_i);

	/**
	 * Verify all challenged segments stored in tic_verify->mkproofs
	 * Output: true if all matched otherwise false
	 */
	bool verifyAllChallenges();
};

#endif
