#include "fps.h"

FPS::FPS(size_t num_sk) {
	resetFPS(num_sk);
}

int FPS::resetFPS(size_t num_sk) {
	std::vector<uchar *> secret_keyPts;
	for (size_t i = 0; i < num_sk; i++) {
		secret_keyPts.push_back((uchar *) COMMON::newRandStr(LAMBDA).data());
	}
	this->resetMERKLE(secret_keyPts, LAMBDA);
	unrevealed_s.clear();
	for (size_t i = 0; i < num_sk; i++) {
		unrevealed_s.push_back(i);
	}
	return FINE;
}

PATH* FPS::newSignature(digest hashvalue) {
	PATH* newSignPt;
	size_t subscript = COMMON::randomNum(hashvalue, unrevealed_s.size());
	std::list<size_t>::iterator torevealIt = unrevealed_s.begin();
	std::advance(torevealIt, subscript);
	newSignPt = this->newPath(*torevealIt);
	torevealIt = unrevealed_s.erase(torevealIt);
	return newSignPt;
}

bool FPS::verifySignature(std::string pubkey,
		std::vector<PATH*> &revealed_v, digest hashvalue, PATH user_sign){
//TODO implement it!
}
