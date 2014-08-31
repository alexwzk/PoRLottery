#ifndef PERMACN_DEALER_H
#define PERMACN_DEALER_H

#include "merkle.h"

#include <fstream>

class DEALER {
private:

	MERKLE *mktreePt = nullptr;
	std::string pubkey;
	size_t num_subset = SUBSET_CONST, num_all = ALL_CONST;
	std::vector<size_t> uarray_pk;

	/**
	 * Generates the S_{pk}
	 * Output: 0 is success, otherwise -1 means not ready yet (no new key);
	 * Affect: updates uarray_pk
	 */
	int createSubset();

public:

	/**
	 * 1st constructor: computes the Merkle tree of the entire dataset
	 * Input: the input file path;
	 * Affect: initialise the merkle_tree
	 */
	DEALER(std::string infile);

	/**
	 * DEALER destructor
	 */
	~DEALER();

	/**
	 *	createSource - setup the permacoin for a participant
	 *	Input: the public string and the output file path
	 *	Output: FINE or FILE_ERR
	 *	Affect: Outsource the {(F{j],PI_j)|j \belongs S_{pk}} to a file
	 */
	int outSource(std::string usr_pubkey, std::string outfile);

	//TODO: copy and assignment functions
};

#endif
