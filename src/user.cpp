#include "user.h"

USER::USER(std::string pubkey) {
	myticket.pubkey = pubkey;
	myticket.seed = COMMON::newRandStr(SEED_LENGTH);
	flags.set(PUBKEY, true);
	flags.set(SEED, true);
}

int USER::generateTicket() {
	if (flags.to_ullong() != 0x0F) { //01111
		std::cout << "failed with flags: " << flags.to_string('0','1') << std::endl;
		return INVALID_ERR;
	}
	std::string tmp_str;
	digest hashvalue;
	myticket.mkproofs.clear();
	for (int i = 0; i < chalng_times; i++) {
		tmp_str = puzzle_id + myticket.pubkey + std::to_string(i)
				+ myticket.seed;
		SHA1(reinterpret_cast<const unsigned char*>(tmp_str.data()),
				tmp_str.size(), hashvalue);
		myticket.mkproofs.push_back(
				allmkproofs[COMMON::hashToNumber(hashvalue) % num_subset]);
	}
	flags.set(TICPROFS, true);
	return SUCCESS;
}

bool USER::isReadyToRelease() {
	return flags.all();
}

int USER::getMKProofs(std::vector<PATH> inmkproofs) {
	allmkproofs = inmkproofs;
	flags.set(ALLPROFS, true);
	return allmkproofs.size();
}

int USER::getNewPuzzle(std::string id) {
	puzzle_id = id;
	flags.set(PUZID, true);
	return SUCCESS;
}

void USER::resetSeed() {
	myticket.seed = COMMON::newRandStr(SEED_LENGTH);
	flags.set(SEED, true);
}

void USER::resetPubkey(std::string newpbkey) {
	myticket.pubkey = newpbkey;
	flags.set(PUBKEY, true);
}

std::string USER::returnMyPubkey() {
	return myticket.pubkey;
}

TICKET* USER::releaseTicket() {
	generateTicket();
	if (!isReadyToRelease()) {
		return nullptr;
	}
	return (&myticket);
}

