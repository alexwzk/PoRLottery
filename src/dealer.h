#ifndef PERMACN_DEALER_H
#define PERMACN_DEALER_H

#include "merkle.h"

#include <fstream>

class DEALER {
private:

	MERKLE *mktreePt;
	std::string pubkey;
	std::vector<size_t> uarray_pk;
	size_t num_subset = SUBSET_CONST, num_all = ALL_CONST;

	/**
	 * Generates the S_{pk}
	 * Output: 0 is success, otherwise -1 means not ready yet (no new key);
	 * Affect: updates uarray_pk
	 */
	int createSubset();

public:

	/**
	 * 1st constructor:computes the Merkle tree of the entire dataset
	 * INPUT the input file path;
	 * AFFECT the mktreePt
	 */
	DEALER(std::string infile);

	/**
	 * DEALER destructor: call MERKLE class destructor
	 */
	~DEALER();

	/**
	 *	Sets up the permacoin for a participant
	 *	INPUT the public string and the output file path
	 *	OUTPUT FINE or FILE_ERR
	 *	AFFECT Outsource the {(F{j],PI_j)|j \belongs S_{pk}} to a file
	 */
	int outSource(std::string usr_pubkey, std::string outfile);

	/**
	 * Releases the Merkle tree root digest to a file/communication channel
	 * Deletgate to the Merkle class then write the binary file
	 */
	int releaseRoot(std::string outfile);

	//TODO: copy and assignment functions
};

#endif
