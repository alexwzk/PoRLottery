#include "user.h"

USER::USER(std::string pubkey) {
	try {
		myticketPt = new TICKET;
	} catch (std::bad_alloc& err) {
		std::cerr << err.what()
				<< " @ User 1st constructor for myticketPt."
				<< std::endl;
		exit(MALLOC_ERR);
	}
	try {
		rand_enginePt = new RANDENGINE();
	} catch (std::bad_alloc& err) {
		std::cerr << err.what()
				<< " @ turning on rand_engine at User 1st constructor."
				<< std::endl;
		exit(MALLOC_ERR);
	}
	myticketPt->pubkey = pubkey;
	myticketPt->seed = rand_enginePt->newRandStr(SEED_LENGTH);
	std::cout << "new rand str: " << myticketPt->seed << std::endl;
	flags.set(PUBKEY, true);
	flags.set(SEED, true);
}

USER::~USER() {
	myticketPt->mkproofs.clear();
	for (auto it : allmkproofPts) {
		delete it;
	}
	allmkproofPts.clear();
	delete myticketPt;
	delete rand_enginePt;
}

int USER::generateTicket() {
	if (flags.to_ullong() != 0x0F) { //01111
		std::cout << "failed with flags: " << flags.to_string('0', '1')
				<< std::endl;
		return INVALID_ERR;
	}
	size_t i_inl = -1;
	myticketPt->mkproofs.clear();
	for (int i = 0; i < i_ink; i++) {
		i_inl = COMMON::computeI_inL(puzzle_id, myticketPt->pubkey, i,
				myticketPt->seed, num_subset);
		std::cout << "Challenge i in l :";
		std::cout << i_inl << " " << std::endl;
		/*std::cout << "Challenge i in n :";
		 std::cout << COMMON::computeU_i(myticket->pubkey,i_inl,ALL_CONST) << std::endl;
		 std::cout << "R_i : " << COMMON::computeR_i(puzzle_id, myticket->pubkey, i,
		 myticket->seed, num_subset, ALL_CONST);*/
		myticketPt->mkproofs.push_back(allmkproofPts[i_inl]);
	}
	flags.set(TICPROFS, true);
	return FINE;
}

bool USER::isReadyToRelease() {
	return flags.all();
}

int USER::storeFile(std::string inputf) {
	using namespace std;
	ifstream inputs;
	PATH* pathPt;
	leaf tmp_leaf;
	digest tmp_dgst;
	size_t vec_size = -1, pth_size = -1;
	try {
		inputs.open(inputf, ifstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ User readPathsFile opening file." << endl;
		exit(FILE_ERR);
	}

	inputs.read((char *) &vec_size, sizeof(vec_size));
	cout << "User received vector size: " << vec_size << endl;
	for (size_t i = 0; i < vec_size; i++) {
		inputs.read((char *) tmp_leaf, LEAF_SIZE);
//		cout << i << " leaf: ";
//		COMMON::printHex(tmp_leaf, LEAF_SIZE);
		try {
			pathPt = new PATH(tmp_leaf);
		} catch (std::bad_alloc& err) {
			std::cerr << err.what() << " @ User storeFile for pathPt."
					<< std::endl;
			exit(MALLOC_ERR);
		}
		inputs.read((char *) &pth_size, sizeof(pth_size));
		cout << "path size: " << pth_size << endl;
		for (size_t j = 0; j < pth_size; j++) {
			inputs.read((char *) tmp_dgst, HASH_SIZE);
			cout << "hash : ";
			COMMON::printHex(tmp_dgst, HASH_SIZE);
			pathPt->pushDigestPt(tmp_dgst);
		}
		allmkproofPts.push_back(pathPt);
	}
	inputs.close();
	flags.set(ALLPROFS, true);
	return FINE;
}

int USER::getNewPuzzle(std::string id) {
	puzzle_id = id;
	flags.set(PUZID, true);
	return FINE;
}

void USER::resetSeed() {
	myticketPt->seed = rand_enginePt->newRandStr(SEED_LENGTH);
	flags.set(SEED, true);
}

void USER::resetPubkey(std::string newpbkey) {
	myticketPt->pubkey = newpbkey;
	flags.set(PUBKEY, true);
}

std::string USER::returnMyPubkey() {
	return myticketPt->pubkey;
}

int USER::writeTicket(std::string outf) {
	using namespace std;
	generateTicket();
	if (!isReadyToRelease()) {
		return INVALID_ERR;
	}
	ofstream outtic;
	size_t tmp_size;
	try {
		outtic.open(outf, ofstream::trunc | ofstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ User releaseTicket opening file." << endl;
		exit(FILE_ERR);
	}
	tmp_size = myticketPt->pubkey.size();
	outtic.write((const char *) &tmp_size, sizeof(size_t));
	outtic.write(myticketPt->pubkey.data(), tmp_size);
	tmp_size = myticketPt->seed.size();
	outtic.write((const char *) &tmp_size, sizeof(size_t));
	outtic.write(myticketPt->seed.data(), tmp_size);
	tmp_size = myticketPt->mkproofs.size();
	outtic.write((const char *) &tmp_size, sizeof(size_t));
	for (auto it : myticketPt->mkproofs) {
		outtic.write((const char *) it->releaseLeafPt(), LEAF_SIZE);
		tmp_size = it->returnSiblings().size();
		outtic.write((const char *) &tmp_size, sizeof(size_t));
		for (auto jt : it->returnSiblings()) {
			outtic.write((const char *) jt, HASH_SIZE);
		}
	}
	outtic.close();
	return FINE;
}

