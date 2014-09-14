#ifndef PERMACN_FPS_H
#define PERMACN_FPS_H

#include "merkle.h"

/**
 * Floating-Preimage Signature
 * FPS owns a Merkle tree
 */
class FPS {

private:
	std::list<size_t> unrevealed_s;
	RANDENGINE* rand_enginePt;
	MERKLE* mktree_keysPt;

public:

	/**
	 * 1st constructor
	 * creates a floating-preimage sginature
	 * INPUT the number of secret keys we need
	 * AFFECT set unrevealed_s = {0,1,2...n}
	 */
	FPS(size_t num_sk);

	/**
	 * Destroctor: delete rand_engine
	 */
	~FPS();

	/**
	 * Generates new FPS signature
	 * INPUT H(m) as a random oracle
	 * OUTPUT signature or nullptr if any error occurs
	 */
	PATH* newSignature(digest hashvalue);

	/**
	 * Return a new copy of the public key
	 * OUTPUT the public key digest
	 */
	uchar* returnPubkey();

	/**
	 * Verify the signature
	 * INPUT a pointer to to-be-verified signature,
	 *       H(m) as a random oracle and the user's signature (mk proof),
	 * 	     User's public key, sk - \Omiga_v (indices whose keys are unrevealed)
	 * OUTPUT true or false
	 * !!!! static member method
	 */
	static bool verifySignature(PATH* signaturePt, digest hashvalue,
			digest pubkey, std::list<size_t> &unrevealed_v);
};

#endif
