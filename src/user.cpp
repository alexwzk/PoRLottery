#include "user.h"

USER::USER() {
	using namespace std;
	try {
		myticketPt = new TICKET;
	} catch (bad_alloc& err) {
		cerr << err.what() << " @ User 1st constructor for myticketPt." << endl;
		exit(MALLOC_ERR);
	}
	try {
		rand_enginePt = new RANDENGINE();
	} catch (bad_alloc& err) {
		cerr << err.what()
				<< " @ turning on rand_engine at User 1st constructor." << endl;
		exit(MALLOC_ERR);
	}
	try {
		fps_schemePt = new FPS(1 << HEIGHT_FPS);
	} catch (bad_alloc& err) {
		cerr << err.what() << " creating FPS @ User 1st constructor." << endl;
		exit(MALLOC_ERR);
	}
	memcpy(myticketPt->pubkey, fps_schemePt->returnPubkey(), HASH_SIZE);
	myticketPt->seed = rand_enginePt->newRandStr(SEED_LENGTH);

	// coutest new rand str
	cout << "new rand str: ";
	COMMON::printHex((const uchar*) myticketPt->seed.data(), SEED_LENGTH);

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

	size_t i_inl = 0, leaf_size = 0;
	PATH *now_signPt = nullptr, *init_signPt = nullptr;
	uchar *leafPt = nullptr;
	uchar empty_leaf[LAMBDA];
	std::string prefix, uchar_str, inputs;
	digest hashvalue;

	myticketPt->mkproofs.clear();
	memset(empty_leaf, 0, LAMBDA);
	init_signPt = now_signPt = new PATH(empty_leaf, LAMBDA);

	uchar_str.assign((char*) myticketPt->pubkey, HASH_SIZE);
	prefix = puzzle_id + uchar_str;

	inputs = prefix + myticketPt->seed;
	i_inl = COMMON::computeI_inL(inputs, num_subset);

	// coutest i_inl_0
	std::cout << "i_inl_0: " << i_inl << std::endl;
//	std::cout << COMMON::stringToHex(inputs) << std::endl;

	for (int i = 0; i < i_ink; i++) {
		//puz || pk || sigma_{i-1} || F[r_i]
		leafPt = allmkproofPts[i_inl]->releaseLeaf(leaf_size);
		uchar_str.assign((char*) leafPt, leaf_size);
		inputs = prefix + now_signPt->returnPathAsStr() + uchar_str;

		//Generate a new signature
		SHA1((uchar*) inputs.data(), inputs.size(), hashvalue);
		now_signPt = fps_schemePt->newSignature(hashvalue);

		//Push_back F[r_i] and sigma_i
		myticketPt->mkproofs.push_back(allmkproofPts[i_inl]);
		myticketPt->fps_signs.push_back(now_signPt);

		//Compute r_{i+1}: puz || pk || sigma_{i}
		inputs = prefix + now_signPt->returnPathAsStr();
		i_inl = COMMON::computeI_inL(inputs, num_subset);

		// coutest i_inl
		std::cout << "i_inl_" << (i + 1) << ": " << i_inl << std::endl;
//		std::cout << COMMON::stringToHex(inputs) << std::endl;
	}
	flags.set(TICPROFS, true);
	delete init_signPt;
	return FINE;
}

bool USER::isReadyToRelease() {
	return flags.all();
}

int USER::storeFile(std::string inputf) {
	using namespace std;
	ifstream inputs;
	PATH* pathPt = nullptr;
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
			pathPt = new PATH(tmp_leaf, LEAF_SIZE);
		} catch (std::bad_alloc& err) {
			std::cerr << err.what() << " @ User storeFile for pathPt."
					<< std::endl;
			exit(MALLOC_ERR);
		}
		inputs.read((char *) &pth_size, sizeof(pth_size));
//		cout << "path size: " << pth_size << endl;
		for (size_t j = 0; j < pth_size; j++) {
			inputs.read((char *) tmp_dgst, HASH_SIZE);
			//coutest hashvalue of the written mkproof
//			cout << "hash : ";
//			COMMON::printHex(tmp_dgst, HASH_SIZE);
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

std::string USER::returnPuzzleID() {
	return puzzle_id;

}

void USER::resetSeed() {
	myticketPt->seed = rand_enginePt->newRandStr(SEED_LENGTH);
	flags.set(SEED, true);
}

uchar* USER::returnMyPubkey() {
	uchar* cp_pubkey = new digest;
	memcpy(cp_pubkey, myticketPt->pubkey, HASH_SIZE);
	return cp_pubkey;
}

int USER::writeTicket(std::string outf) {
	using namespace std;
	generateTicket();
	if (!isReadyToRelease()) {
		cerr << "Not ready to release @ User writeTicket." << endl;
		return INVALID_ERR;
	}
	ofstream outtic;
	try {
		outtic.open(outf, ofstream::trunc | ofstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << "Open file @ User writeTicket." << endl;
		exit(FILE_ERR);
	}
	outtic << (*myticketPt);
	outtic.close();
	return FINE;
}

