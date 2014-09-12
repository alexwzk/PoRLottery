#ifndef PERMACN_FPS_H
#define PERMACN_FPS_H

#include "merkle.h"

/**
 * Floating-Preimage Signature is a signature Merkle tree
 * FPS owns a Merkle tree
 */
class FPS: public MERKLE {

private:
	std::list<size_t> unrevealed_s;

public:

	/**
	 * default constructor
	 */
	FPS() {

	}

	/**
	 * 1st constructor
	 * GOTO resetFPS
	 */
	FPS(size_t num_sk);

	/**
	 * creates a floating-preimage sginature
	 * INPUT the number of secret keys we need
	 * AFFECT set unrevealed_s = {0,1,2...n}
	 * OUTPUT fine
	 */
	int resetFPS(size_t num_sk);

	/**
	 * Generates new FPS signature
	 * INPUT H(m) as a random oracle
	 * OUTPUT signature or nullptr if any error occurs
	 */
	PATH* newSignature(digest hashvalue);

	/**
	 * Verify the signature
	 * INPUT User's public key, \Omiga_v (revealed keys to verifier),
	 * 		 H(m) as a random oracle and the user's signature (mk proof)
	 * 		 to-be-verified signature
	 * OUTPUT true or false
	 * !!!! static member method
	 */
	static bool verifySignature(std::string pubkey,
			std::vector<PATH*> &revealed_v, digest hashvalue, PATH user_sign);
};

#endif
