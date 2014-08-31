#include "user.h"

USER::USER(std::string pubkey) {
	myticket = new TICKET;
	myticket->pubkey = pubkey;
	myticket->seed = COMMON::newRandStr(SEED_LENGTH);
	flags.set(PUBKEY, true);
	flags.set(SEED, true);
}

USER::~USER() {
	myticket->mkproofs.clear();
	for (auto it : allmkproofs) {
		delete it;
	}
	allmkproofs.clear();
	delete myticket;
}

int USER::generateTicket() {
	if (flags.to_ullong() != 0x0F) { //01111
		std::cout << "failed with flags: " << flags.to_string('0', '1')
				<< std::endl;
		return INVALID_ERR;
	}
	std::string tmp_str;
	digest hashvalue;
	myticket->mkproofs.clear();
	for (int i = 0; i < chalng_times; i++) {
		tmp_str = puzzle_id + myticket->pubkey + std::to_string(i)
				+ myticket->seed;
		SHA1((const uchar*) (tmp_str.data()), tmp_str.size(), hashvalue);
		myticket->mkproofs.push_back(
				allmkproofs[COMMON::hashToNumber(hashvalue) % num_subset]);
	}
	flags.set(TICPROFS, true);
	return FINE;
}

bool USER::isReadyToRelease() {
	return flags.all();
}

int USER::readPathsFile(std::string inputf) {
	using namespace std;
	ifstream inputs;
	PATH* pathPt;
	leaf tmp_leaf;
	digest tmp_dgst;
	size_t  vec_size = -1, pth_size = -1;
	try {
		inputs.open(inputf, ifstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at User readPathsFile." << endl;
		exit(FILE_ERR);
	}

	inputs.read((char *) &vec_size, sizeof(vec_size));
	cout << "vector size: " << vec_size << endl;
	for (size_t i = 0; i < vec_size; i++) {
		inputs.read((char *) tmp_leaf, LEAF_SIZE);
		cout << i <<" leaf: ";
		COMMON::printHex(tmp_leaf, LEAF_SIZE);
		pathPt = new PATH(tmp_leaf);
		inputs.read((char *) &pth_size, sizeof(pth_size));
		cout << "path size: " << pth_size << endl;
		for (size_t j = 0; j < pth_size; j++) {
			inputs.read((char *) tmp_dgst, HASH_SIZE);
			cout << "hash : ";
			COMMON::printHex(tmp_dgst, HASH_SIZE);
			pathPt->pushDigestPt(tmp_dgst);
		}
		allmkproofs.push_back(pathPt);
	}
	inputs.close();
	flags.set(ALLPROFS,true);
	return FINE;
}

int USER::getNewPuzzle(std::string id) {
	puzzle_id = id;
	flags.set(PUZID, true);
	return FINE;
}

void USER::resetSeed() {
	myticket->seed = COMMON::newRandStr(SEED_LENGTH);
	flags.set(SEED, true);
}

void USER::resetPubkey(std::string newpbkey) {
	myticket->pubkey = newpbkey;
	flags.set(PUBKEY, true);
}

std::string USER::returnMyPubkey() {
	return myticket->pubkey;
}

int USER::releaseTicket(std::string outf) {
	using namespace std;
	generateTicket();
	if (!isReadyToRelease()) {
		return INVALID_ERR;
	}
	ofstream outtic;
	try {
		outtic.open(outf, ofstream::trunc);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at User releaseTicket."
				<< endl;
		exit (FILE_ERR);
	}
	outtic << myticket->pubkey;
	outtic << myticket->seed;
	outtic << myticket->mkproofs.size();
	for(auto it : myticket->mkproofs){
		outtic.write((const char *)it->returnLeafPt(),LEAF_SIZE);
		outtic << it->returnSiblings().size();
		for(auto jt : it->returnSiblings()){
			outtic.write((const char *)jt,HASH_SIZE);
		}
	}
	outtic.close();
	return FINE;
}

