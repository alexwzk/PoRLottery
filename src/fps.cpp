#include "fps.h"

FPS::FPS(size_t num_sk) {
	try {
		rand_enginePt = new RANDENGINE();
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << " @ FPS 1st constructor for [rand_enginePt]."
				<< std::endl;
	}
	std::vector<uint8_t *> seckeyPts;
	uint8_t* tmp_seckeyPt;
	for (size_t i = 0; i < num_sk; i++) {
		try {
			tmp_seckeyPt = new uint8_t[LAMBDA];
		} catch (std::bad_alloc& err) {
			std::cerr << err.what()
					<< " @ FPS 1st constructor for [tmp_seckeyPt]."
					<< std::endl;
		}
		memcpy(tmp_seckeyPt, (uint8_t *) rand_enginePt->newRandStr(LAMBDA).data(),
		LAMBDA);
		seckeyPts.push_back(tmp_seckeyPt);
	}
	//coutest the secert key leaves
	/*std::cout << "seckey leaves are: " << std::endl;
	 for (uchar* it : seckeyPts) {
	 COMMON::printHex(it, LAMBDA);
	 }*/

	try {
		mktree_keysPt = new MERKLE(seckeyPts, LAMBDA);
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << " @ FPS 1st constructor for [mktree_keysPt]."
				<< std::endl;
	}
	unrevealed_s.clear();
	for (size_t i = 0; i < num_sk; i++) {
		unrevealed_s.push_back(i);
	}
}

FPS::~FPS() {
	delete mktree_keysPt;
	delete rand_enginePt;
}

PATH* FPS::newSignature(digest hashvalue) {
	PATH* newSignPt = nullptr;
//	std::cout<< "Here is newSignature." << std::endl;
	size_t index = rand_enginePt->randByHash(hashvalue, unrevealed_s.size());
	// coutest rand subscript
	//std::cout << "The rand subscript is : " << index << std::endl;
	std::list<size_t>::iterator torevealIt = unrevealed_s.begin();
	std::advance(torevealIt, index);
	newSignPt = mktree_keysPt->newPath(*torevealIt);
	torevealIt = unrevealed_s.erase(torevealIt);
	std::cout << "fetch No." << (*torevealIt) << " signature." << std::endl;
	return newSignPt;
}

std::vector<PATH*> FPS::rewardSignatures(size_t k_reveal, size_t k_select){
	std::vector<PATH*> sign;
	std::vector<size_t> selected_s;
	sign.reserve(k_reveal);
	selected_s.reserve(k_select);
	//TODO continue implementing (k+1)th signature
	return sign;
}

uint8_t* FPS::returnPubkey() {
	if (mktree_keysPt->releaseRootPt() == nullptr) {
		return nullptr;
	}
	uint8_t* pubkeyPt = nullptr;
	try {
		pubkeyPt = new uint8_t[HASH_SIZE];
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << " @ FPS returnPubkey for [pubkeyPt]."
				<< std::endl;
	}
	memcpy(pubkeyPt, mktree_keysPt->releaseRootPt(), HASH_SIZE);
	return pubkeyPt;
}

bool FPS::verifySignature(PATH* signaturePt, digest hashvalue, digest pubkey,
		std::list<size_t> &unrevealed_v, RANDENGINE &rand_engine) {
	bool passed = false;
//	std::cout << "Here is verifySignature." << std::endl;
	size_t index = rand_engine.randByHash(hashvalue, unrevealed_v.size());
	std::list<size_t>::iterator torevealIt = unrevealed_v.begin();
	std::advance(torevealIt, index);
	passed = MERKLE::verifyPath(signaturePt, (*torevealIt), pubkey);
	torevealIt = unrevealed_v.erase(torevealIt);
	std::cout << "verified No." << (*torevealIt) << " signature." << std::endl;
	return passed;
}
