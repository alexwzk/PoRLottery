#include "fps.h"

FPS::FPS(size_t num_sk) {
	try {
		rand_enginePt = new RANDENGINE();
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << " @ FPS 1st constructor for [rand_enginePt]."
				<< std::endl;
	}
	std::vector<uchar *> seckeyPts;
	uchar* tmp_seckeyPt;
	for (size_t i = 0; i < num_sk; i++) {
		try {
			tmp_seckeyPt = new uchar[LAMBDA];
		} catch (std::bad_alloc& err) {
			std::cerr << err.what()
					<< " @ FPS 1st constructor for [tmp_seckeyPt]."
					<< std::endl;
		}
		memcpy(tmp_seckeyPt, (uchar *) rand_enginePt->newRandStr(LAMBDA).data(),
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
	size_t index = rand_enginePt->randByHash(hashvalue, unrevealed_s.size());
	// coutest rand subscript
	std::cout << "The rand subscript is : " << index << std::endl;
	std::list<size_t>::iterator torevealIt = unrevealed_s.begin();
	std::advance(torevealIt, index);
	newSignPt = mktree_keysPt->newPath(*torevealIt);
	torevealIt = unrevealed_s.erase(torevealIt);
	return newSignPt;
}

uchar* FPS::returnPubkey() {
	if (mktree_keysPt->releaseRootPt() == nullptr) {
		return nullptr;
	}
	uchar* pubkeyPt = nullptr;
	try {
		pubkeyPt = new uchar[HASH_SIZE];
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << " @ FPS returnPubkey for [pubkeyPt]."
				<< std::endl;
	}
	memcpy(pubkeyPt, mktree_keysPt->releaseRootPt(), HASH_SIZE);
	return pubkeyPt;
}

bool FPS::verifySignature(PATH* signaturePt, digest hashvalue, digest pubkey,
		std::list<size_t> &unrevealed_v) {
	bool passed = false;
	size_t index = rand_enginePt->randByHash(hashvalue, unrevealed_v.size());
	std::list<size_t>::iterator torevealIt = unrevealed_v.begin();
	std::advance(torevealIt, index);
	passed = MERKLE::validatePath(signaturePt, LAMBDA, (*torevealIt), pubkey);
	torevealIt = unrevealed_v.erase(torevealIt);
	return passed;
}
