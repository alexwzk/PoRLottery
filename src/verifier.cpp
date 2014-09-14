#include "verifier.h"

VERIFIER::VERIFIER(std::string root_file, std::string tic_file) {
	using namespace std;
	ifstream inticket, inroot;
	size_t buf_nsize = -1, buf_osize = -1, chalng_times = -1, path_len = -1;
	leaf tmp_leaf;
	digest tmp_digest;
	char* bufferPt = nullptr;
	PATH* pathPtr = nullptr;
	try {
		inticket.open(tic_file);
	} catch (ifstream::failure& err) {
		cerr << err.what()
				<< " @ Opening ticket file at Verifier 2nd constructor."
				<< endl;
		exit(FILE_ERR);
	}
	try {
		inroot.open(root_file);
	} catch (ifstream::failure& err) {
		cerr << err.what()
				<< " @ Opening root file at Verifier 2nd constructor." << endl;
		exit(FILE_ERR);
	}
	root_digest = new digest;
	inroot.read((char *) root_digest, HASH_SIZE);
	cout << "root: ";
	COMMON::printHex(root_digest, HASH_SIZE);
	inroot.close();

	tic_verify = new TICKET;
	inticket.read((char *) &buf_nsize, sizeof(size_t));
	bufferPt = new char[buf_nsize];
	inticket.read(bufferPt, buf_nsize);
	tic_verify->pubkey.append(bufferPt, bufferPt + buf_nsize);
	cout << "pk: " << tic_verify->pubkey << endl;
	buf_osize = buf_nsize;
	inticket.read((char *) &buf_nsize, sizeof(size_t));
	if (buf_nsize > buf_osize) {
		delete[] bufferPt;
		bufferPt = new char[buf_nsize];
	}
	inticket.read(bufferPt, buf_nsize);
	tic_verify->seed.append(bufferPt, bufferPt + buf_nsize);
	cout << "sd: " << tic_verify->seed << endl;
	delete[] bufferPt;
	inticket.read((char *) &chalng_times, sizeof(size_t));
	cout << "ct: " << chalng_times << endl;
	for (size_t i = 0; i < chalng_times; i++) {
		inticket.read((char *) tmp_leaf, LEAF_SIZE);
//		cout << "leaf: ";
//		COMMON::printHex(tmp_leaf, LEAF_SIZE);
		pathPtr = new PATH(tmp_leaf);
		inticket.read((char *) &path_len, sizeof(size_t));
		for (size_t j = 0; j < path_len; j++) {
			inticket.read((char *) tmp_digest, HASH_SIZE);
			cout << "hash: ";
			COMMON::printHex(tmp_digest, HASH_SIZE);
			pathPtr->pushDigestPt(tmp_digest);
		}
		tic_verify->mkproofs.push_back(pathPtr);
	}
	inticket.close();
}

VERIFIER::~VERIFIER() {
	delete root_digest;
	for (auto it : tic_verify->mkproofs) {
		delete it;
	}
	delete tic_verify;
}

int VERIFIER::getPuzzleID(std::string puz_id) {
	puzzle_id = puz_id;
	return FINE;
}

bool VERIFIER::verifyAllChallenges() {
	int i_ink = tic_verify->mkproofs.size();
	size_t r_i;
	for (int i = 0; i < i_ink; i++) {
		r_i = COMMON::computeR_i(puzzle_id, tic_verify->pubkey, i,
				tic_verify->seed, num_subset, num_all);
		std::cout << " challenge U_i: " << r_i << std::endl;
		if (!MERKLE::validatePath(tic_verify->mkproofs[i], LEAF_SIZE, r_i, root_digest)) {
			return false;
		}
	}
	return true;
}
