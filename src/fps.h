/*
 * fps.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */
#ifndef PMC_FPS_H_
#define PMC_FPS_H_

#include "merkle.h"

/**
 * Floating-Preimage Signature Scheme
 * FPS owns a Merkle tree
 */
template<unsigned int FPS_LFBYTES>
class FPS {

private:
	std::list<size_t> unrevealed_s;
	size_t num_leaves, num_select, num_reveal;
	MERKLE<FPS_LFBYTES>* mktree_keysPt;

public:

	/**
	 * 1st constructor: creates a floating-preimage sginature
	 * INPUT the number of secret keys we need
	 * AFFECT set unrevealed_s = {0,1,2...n}
	 */
	FPS(const size_t num_leaves, const size_t num_select,
			const size_t num_reveal) {
		this->num_leaves = num_leaves;
		this->num_select = num_select;
		this->num_reveal = num_reveal;

		std::vector< BUFFER<FPS_LFBYTES> > fps_leaves;
		fps_leaves.resize(num_leaves);
		for (size_t i = 0; i < num_leaves; i++) {
			GetRandBytes(fps_leaves[i].data, FPS_LFBYTES);
		}

		try {
			mktree_keysPt = new MERKLE<FPS_LFBYTES>(fps_leaves);
		} catch (std::bad_alloc& err) {
			throw error(err.what());
		}

		unrevealed_s.clear();
		for (size_t i = 0; i < num_leaves; i++) {
			unrevealed_s.push_back(i);
		}
	}

	/**
	 * Destroctor: deletes rand_engine and mktree_keys
	 */
	~FPS() {
		delete mktree_keysPt;
	}

	/**
	 * Generates a new FPS signature
	 * INPUT H(m) as a random oracle
	 * OUTPUT signature or NULL if any error occurs
	 */
	PATH<FPS_LFBYTES> returnSign(const uint256& hashvalue) {
		PATH<FPS_LFBYTES> nsign;
		size_t index = PMC::getRandByHash(hashvalue,
				unrevealed_s.size()-1);
		std::list<size_t>::iterator torevealIt = unrevealed_s.begin();
		std::advance(torevealIt, index);
		nsign = mktree_keysPt->returnPath(*torevealIt);
		torevealIt = unrevealed_s.erase(torevealIt);
		// coutest revealed No.
		/*std::cout << "revealed No." << (*torevealIt) << " signature."
				<< std::endl;*/
		return nsign;
	}

	/**
	 * TODO User selects
	 * the (k+1)th FPS signature for paying the Bitcoin reward
	 * OUTPUT num_reveal keys
	 */
	std::vector< PATH<FPS_LFBYTES> > rewardSign(std::vector< uint256 >& reveal_seeds){
		std::vector< PATH<FPS_LFBYTES> > nreward_signs;
		reveal_seeds.clear();
		reveal_seeds.reserve(num_reveal);
		nreward_signs.reserve(num_reveal);
		for(size_t i = 0; i < num_reveal; i++){
			reveal_seeds[i] = GetRandHash();
			nreward_signs[i] = this->returnSign(reveal_seeds[i]);
		}
		return nreward_signs;
	}

	/**
	 * Returns a new copy of the public key
	 * OUTPUT the public key digest
	 */
	uint256 returnPubkey() const {
		return mktree_keysPt->returnRoot();
	}

	/**
	 * Verifies the signature
	 * INPUT a pointer to to-be-verified signature,
	 *       H(m) as a random oracle and the user's signature (mk proof),
	 * 	     User's public key, sk - \Omiga_v (indices whose keys are unrevealed)
	 * OUTPUT true or false
	 */
	static bool verifySignature(const PATH<FPS_LFBYTES>& vsign,
			const uint256& hashvalue, const uint256& pubkey,
			std::list<size_t>& unrevealed_v) {
		bool passed = false;
		size_t index = PMC::getRandByHash(hashvalue, unrevealed_v.size()-1);
		std::list<size_t>::iterator torevealIt = unrevealed_v.begin();
		std::advance(torevealIt, index);
		passed = MERKLE<FPS_LFBYTES>::verifyPath(vsign,
				(*torevealIt), pubkey);
		torevealIt = unrevealed_v.erase(torevealIt);
		// coutest verified No.
		/*std::cout << "verified No." << (*torevealIt) << " signature."
				<< std::endl;*/
		return passed;
	}
};

#endif /*PMC_FPS_H_*/
